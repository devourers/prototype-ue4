// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 3.0f;
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSceneComponent"));
	}
	if (!CollisionComponent) {
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Bullet"));
		CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
		CollisionComponent->InitSphereRadius(15.0f);
		RootComponent = CollisionComponent;
	}
	if (!BulletMovementComponent) {
		//Move all of this to .h
		BulletMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
		BulletMovementComponent->SetUpdatedComponent(CollisionComponent);
		BulletMovementComponent->InitialSpeed = 3000.0f;
		BulletMovementComponent->MaxSpeed = 3000.0f;
		BulletMovementComponent->bRotationFollowsVelocity = true;
		BulletMovementComponent->bShouldBounce = true;
		BulletMovementComponent->Bounciness = 0.3f;
		BulletMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!BulletMeshComponent) {
		BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Meshes/Sphere.Sphere'"));
		if (Mesh.Succeeded()) {
			BulletMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Materials/BulletMaterial.BulletMaterial'"));
	if (Material.Succeeded()) {
		BulletMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, BulletMeshComponent);
	}
	BulletMeshComponent->SetMaterial(0, BulletMaterialInstance);
	BulletMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	BulletMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::FireInDirection(const FVector& ShootDirection) {
	BulletMovementComponent->Velocity = ShootDirection * BulletMovementComponent->InitialSpeed;
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) {
		OtherComponent->AddImpulseAtLocation(BulletMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}

	Destroy();
}