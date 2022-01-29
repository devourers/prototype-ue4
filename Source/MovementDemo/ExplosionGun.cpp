// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGun.h"

// Sets default values
AExplosionGun::AExplosionGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSceneComponent"));
	}
	ExGunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = ExGunMeshComponent;

}

// Called when the game starts or when spawned
void AExplosionGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosionGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosionGun::Fire(const FVector& MuzzleLocation, const FRotator& MuzzleRotation) {
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Fired"));
	UWorld* World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		AExplodingProjectile* SpawnedBullet = World->SpawnActor<AExplodingProjectile>(MuzzleLocation, MuzzleRotation, SpawnParams);
		if (SpawnedBullet) {
			FVector LaunchDirection = MuzzleRotation.Vector();
			SpawnedBullet->FireInDirection(LaunchDirection);
		}
	}
}
