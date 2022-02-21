// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "WeaponBase.h"
#include "CableComponent.h"
#include "InteractableObject.h"
#include "ProtagClass.generated.h"

UCLASS()
class MOVEMENTDEMO_API AProtagClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProtagClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;
	void AddControllerPitchYawInput(float pitch, float yaw);

	UFUNCTION()
		void MoveRight(float value);

	UFUNCTION()
		void MoveForward(float value);

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	UFUNCTION()
		void ApplyBhop();

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void Reload();
	
	UFUNCTION()
		void SwitchWeapons();

	UFUNCTION()
		void CanSwitchAgain();

	UFUNCTION()
		void Interact();

	UFUNCTION()
		void ToggleItemPick();

	UFUNCTION()
		void ToggleCrouch();

	UFUNCTION()
		void Lean(float v);

	UFUNCTION()
		void SetCameraPos();

	UFUNCTION()
		void ShootRope();

	UFUNCTION()
		void RopeLonger();

	UFUNCTION()
		void RopeShorter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int current_weapon;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ProtagCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		USkeletalMeshComponent* ProtagMesh;

	UPROPERTY(VisibleAnywhere)
		UCharacterMovementComponent* ProtagMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Inventory)
		TArray<TSubclassOf<class AWeaponBase>> WeaponInventoryClasess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Inventory)
		TArray<class AWeaponBase*> WeaponInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health;

	//Interacting with items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USceneComponent* HoldingComponent;

	//Interacting with ropes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USceneComponent* RopeHoldingComponent;

	//RopeGun attachment component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USceneComponent* RopeGunComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float rope_range;

	UPROPERTY()
		bool isHolding;

	UPROPERTY()
		bool isRopeGunned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RopeGun)
		class UCableComponent* CurrentCable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RopeGun)
		class ABaseProjectile* RopeGunShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RopeGun)
		TSubclassOf<class ABaseProjectile> RopeGunProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = RopeGun)
		class ABaseProjectile* RopeGunProjectile;

	UPROPERTY(EditAnywhere)
		class AInteractableObject* CurrentItem;

	//Timer Handlers
	FTimerHandle BhopHandler;
	FTimerHandle SwitchHandler;

	//gameplay in-code logic
	FVector InteractStart;
	FVector InteractFinish;
	FVector ForwardVector;
	FHitResult InteractHit;
	FComponentQueryParams DefaultComponentQueryParams;
	FCollisionResponseParams DefaultResponseParam;
	bool can_bhop = false;
	bool can_switch = true;
	bool is_crouching = false;
	bool is_leaning = false;
	bool is_sprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MaxLeanAngle = 35;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float LeanHeightRatio = 0.5;

	float LeanAmount = 0;
};
