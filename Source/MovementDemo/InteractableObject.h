// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "InteractableObject.generated.h"

UCLASS()
class MOVEMENTDEMO_API AInteractableObject : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AInteractableObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* HoldingComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionComponent;

	UFUNCTION()
		void Pickup();

	UFUNCTION()
		void Launch();

	bool bHolding;
	bool launched;
	
	FRotator ControlRotation;
	ACharacter* ProtagChar;
	UCameraComponent* PlayerCamera;
	FVector ForwardVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
