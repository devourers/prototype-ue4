// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "WeaponBase.h"
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

	UPROPERTY()
		TArray<class AWeaponBase*> WeaponInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health;

	//Interacting with items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USceneComponent* HoldingComponent;

	UPROPERTY()
		bool isHolding;

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
};
