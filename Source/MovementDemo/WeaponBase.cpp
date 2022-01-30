// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSceneComponent"));
	}
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = WeaponMeshComponent;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::AttackWithWeapon(const FVector& MuzzleLocation, const FRotator& MuzzleRotation) {
	if (!isReloading) {
		if (wType == WeaponTypes::Melee) {
			//TODO
		}
		else if (wType == WeaponTypes::RangedProjectile) {
			check(GEngine != nullptr);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Fired"));
			UWorld* World = GetWorld();
			if (World) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();
				ABaseProjectile* SpawnedBullet = World->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (SpawnedBullet) {
					FVector LaunchDirection = MuzzleRotation.Vector();
					SpawnedBullet->FireInDirection(LaunchDirection);
				}
			}
		}
		else if (wType == WeaponTypes::RangedHitScan) {
			//TODO
		}
	}
}

void AWeaponBase::StopReload() {
	isReloading = false;
}

void AWeaponBase::Reload() {
	if (!isReloading) {
		isReloading = true;
		AmmoCount -= Magazine - CurrentAmmo;
		CurrentAmmo = Magazine;
		//play animation;
		GetWorldTimerManager().SetTimer(ReloadHandler, this, &AWeaponBase::StopReload, 0.2f, false, 1.0f);
	}
}