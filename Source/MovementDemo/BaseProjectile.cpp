// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include <string>

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//InitialLifeSpan = 3.0f; //recount from range
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PrSceneComponent"));
	}
	if (!CollisionComponent) {
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Bullet"));
		CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
		CollisionComponent->InitSphereRadius(15.0f);
		RootComponent = CollisionComponent;
	}
	if (!PrMovementComponent) {
		PrMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
		PrMovementComponent->SetUpdatedComponent(CollisionComponent);
		PrMovementComponent->InitialSpeed = 4000.0f;
		PrMovementComponent->MaxSpeed = 4000.0f;
		PrMovementComponent->bRotationFollowsVelocity = true;
		PrMovementComponent->bShouldBounce = true;
		PrMovementComponent->Bounciness = 0.3f;
		PrMovementComponent->ProjectileGravityScale = 0.0f;
	}
	if (!PrMeshComponent) {
		PrMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Meshes/Sphere.Sphere'"));
		if (Mesh.Succeeded()) {
			PrMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Materials/BulletMaterial.BulletMaterial'"));
	if (Material.Succeeded()) {
		PrMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, PrMeshComponent);
	}
	PrMeshComponent->SetMaterial(0, PrMaterialInstance);
	PrMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	PrMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	StartingPosition = GetActorLocation();
	LastLocation = StartingPosition;
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrPosition = GetActorLocation();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, LastLocation.ToString());
	TraveledDistance += (CurrPosition - LastLocation).Size();
	LastLocation = CurrPosition;
	if (TraveledDistance >= Range) {
		Destroy();
	}
}

void ABaseProjectile::FireInDirection(const FVector& ShootDirection) {
	PrMovementComponent->Velocity = ShootDirection * PrMovementComponent->InitialSpeed;
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherComponent->IsCollisionEnabled()){
		if (pType == ProjectileTypes::Rope) {
			TraveledDistance += (GetActorLocation() - LastLocation).Size();
			LastHitActor = OtherActor;
			HitLocation = Hit.ImpactPoint;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, HitLocation.ToString());
			OnProjectileHit.ExecuteIfBound();
			Destroy();
		}
		else if (pType == ProjectileTypes::Normal) {
			if (!DoesRichochet) {
				Destroy();
			}
			if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) {
				OtherComponent->AddImpulseAtLocation(PrMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
			}

		}
		else if (pType == ProjectileTypes::Explode) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, LastLocation.ToString());
			Destroy();
			FCollisionShape onHitColl = FCollisionShape::MakeSphere(Radius);
			FVector MyLocation = GetActorLocation();
			FVector Start = MyLocation;
			FVector End = MyLocation;
			TArray<FHitResult> OutHits;
			bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_WorldStatic, onHitColl);
			if (ProjectileEffect != nullptr){
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileEffect, MyLocation);
			}
			for (auto& Hit : OutHits) {
				auto* HitComp = Hit.GetComponent();
				if (Hit.GetActor()->CanBeDamaged()) {
					FRadialDamageEvent ev;
					float DamageFalloff = 1.0f;
					DamageFalloff = FMath::Clamp(DamageFalloff, (Hit.GetActor()->GetActorLocation() - MyLocation).Size() / Radius, 1.0f);
					Hit.GetActor()->TakeDamage(Damage * DamageFalloff, ev, GetInstigatorController(), this);
				}
				if (HitComp && HitComp->IsSimulatingPhysics() == true) {
					HitComp->AddRadialImpulse(Hit.ImpactPoint, Radius, PrMovementComponent->Velocity.Size() * 10.0f, ERadialImpulseFalloff::RIF_Constant);
				}
			}
		}
	}
}