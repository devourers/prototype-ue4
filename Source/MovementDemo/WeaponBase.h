// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.h"
#include "WeaponBase.generated.h"

UENUM()
enum WeaponTypes {
	RangedProjectile UMETA(DisplayName = "Ranged Projectile"),
	RangedHitScan UMETA(DisplayName = "Ranged Hitscan"),
	Melee UMETA(DisplayName = "Melee")
};

UCLASS()
class MOVEMENTDEMO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Weapon Type ")
		FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Weapon Type ")
		TEnumAsByte<WeaponTypes> wType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Mesh ")
		USkeletalMeshComponent* WeaponMeshComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Projectile ")
		TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Ammo ")
		int Magazine;

	UPROPERTY()
		int AmmoCount;

	UPROPERTY()
		int CurrentAmmo;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		bool isReloading;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AttackWithWeapon(const FVector& MuzzleLocation, const FRotator& MuzzleRotation);
	void Reload();
	void StopReload();
	FTimerHandle ReloadHandler;
};
