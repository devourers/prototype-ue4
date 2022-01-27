// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Bullet.h"
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
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ABullet> BulletClass;


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
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ProtagCameraComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* ProtagMesh;
	UPROPERTY(VisibleAnywhere)
		UCharacterMovementComponent* ProtagMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	bool can_bhop = false;

	FTimerHandle BhopHandler;
};
