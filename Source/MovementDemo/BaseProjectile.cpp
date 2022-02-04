// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

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
		PrMovementComponent->InitialSpeed = 3000.0f;
		PrMovementComponent->MaxSpeed = 3000.0f;
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
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ((GetActorLocation() - StartingPosition).Size() >= Range) {
		Destroy();
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Out of range"));
	}
}

void ABaseProjectile::FireInDirection(const FVector& ShootDirection) {
	PrMovementComponent->Velocity = ShootDirection * PrMovementComponent->InitialSpeed;
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if (pType == ProjectileTypes::Normal) {
		if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) {
			OtherComponent->AddImpulseAtLocation(PrMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
		}
		if (!DoesRichochet){
			Destroy();
		}
	}
	else if (pType == ProjectileTypes::Explode) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Firing Projectile"));
		FCollisionShape onHitColl = FCollisionShape::MakeSphere(600.0f);
		FVector MyLocation = GetActorLocation();
		FVector Start = MyLocation;
		FVector End = MyLocation;
		TArray<FHitResult> OutHits;
		bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_WorldStatic, onHitColl);
		for (auto& Hit : OutHits) {
			auto* HitComp = Hit.GetComponent();
			if (Hit.GetActor()->CanBeDamaged()) {
				FRadialDamageEvent ev;
				Hit.GetActor()->TakeDamage(Damage, ev, GetInstigatorController(), this);
			}
			if (HitComp) {
				HitComp->AddRadialImpulse(Hit.ImpactPoint, 600.0f, PrMovementComponent->Velocity.Size() * 10.0f, ERadialImpulseFalloff::RIF_Constant);
			}
		}

		Destroy();
	}
}