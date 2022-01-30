// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "ExplosionGun.h"
#include "Bullet.h"
#include "WeaponBase.h"
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
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ProtagCameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		USkeletalMeshComponent* ProtagMesh;
	UPROPERTY(VisibleAnywhere)
		UCharacterMovementComponent* ProtagMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<class AExplosionGun> ExpGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class AExplosionGun* Gun;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Audio)
		USoundBase* SB_shot;
	bool can_bhop = false;

	FTimerHandle BhopHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AWeaponBase*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int EquippedWeapon;
};
