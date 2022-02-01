// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum EWeaponTypes {
	eWT_RangedProjectile UMETA(DisplayName = "Ranged Projectile"),
	eWT_RangedHitScan UMETA(DisplayName = "Ranged Hitscan"),
	eWT_Melee UMETA(DisplayName = "Melee")
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
		TEnumAsByte<EWeaponTypes> wType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Mesh ")
		USkeletalMeshComponent* WeaponMeshComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Projectile ")
		TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Projectile ")
		class ABaseProjectile* Projectile;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Ammo ")
		int Magazine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Ammo ")
		int AmmoCount;

	UPROPERTY()
		int CurrentAmmo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Damage ")
		float Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Audio)
		USoundBase* SB_shot;

	UFUNCTION()
		void HideWeapon(bool to_hide);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		bool isReloading;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void AttackWithWeapon(const FVector& MuzzleLocation, const FRotator& MuzzleRotation);
	virtual void Reload();
	virtual void StopReload();
	FTimerHandle ReloadHandler;
};
