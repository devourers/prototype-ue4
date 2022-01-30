// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodingProjectile.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

// Sets default values
AExplodingProjectile::AExplodingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ExProjectileSceneComponent"));
	}
	if (!CollisionComponent) {
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Bullet"));
		CollisionComponent->OnComponentHit.AddDynamic(this, &AExplodingProjectile::OnHit);
		CollisionComponent->InitSphereRadius(15.0f);
		RootComponent = CollisionComponent;
	}
	if (!ExProjectileMovementComponent) {
		ExProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
		ExProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ExProjectileMovementComponent->InitialSpeed = 3000.0f;
		ExProjectileMovementComponent->MaxSpeed = 3000.0f;
		ExProjectileMovementComponent->bRotationFollowsVelocity = true;
		ExProjectileMovementComponent->bShouldBounce = true;
		ExProjectileMovementComponent->Bounciness = 0.3f;
		ExProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	if (!ProjectileMeshComponent) {
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Meshes/Sphere.Sphere'"));
		if (Mesh.Succeeded()) {
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Materials/BulletMaterial.BulletMaterial'"));
	if (Material.Succeeded()) {
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AExplodingProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplodingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplodingProjectile::FireInDirection(const FVector& ShootDirection) {
	ExProjectileMovementComponent->Velocity = ShootDirection * ExProjectileMovementComponent->InitialSpeed;
}

void AExplodingProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	FCollisionShape onHitColl = FCollisionShape::MakeSphere(600.0f);
	FVector MyLocation = GetActorLocation();
	FVector Start = MyLocation;
	FVector End = MyLocation;
	TArray<FHitResult> OutHits;

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_WorldStatic, onHitColl);
	for (auto& Hit : OutHits) {
		UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>((Hit.GetActor())->GetRootComponent());
		if (MeshComp) {
			MeshComp->AddRadialImpulse(Hit.ImpactPoint, 600.0f, ExProjectileMovementComponent->Velocity.Size() * 100.0f, ERadialImpulseFalloff::RIF_Constant);
			/*
			ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(Hit.Location, FRotator());
			if (decal) {
				//decal->SetDecalMaterial()
				decal->SetLifeSpan(2.0f);
				decal->GetDecal()->DecalSize = FVector(32.0f, 64.0f, 64.0f);
			}
			*/
		}
	}

	Destroy();
}