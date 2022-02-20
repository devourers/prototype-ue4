// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "Components/SphereComponent.h"
#include "SwingingRope.generated.h"

UCLASS()
class MOVEMENTDEMO_API ASwingingRope : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rope)
		UCableComponent* RopeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rope)
		TArray<class USphereComponent*> GrabPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rope)
		USceneComponent* CurrentGrabPlace;

	ASwingingRope();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void GetGrabbed(const USphereComponent& GrabbedSphere);
	UFUNCTION()
		void MoveUp();
	UFUNCTION()
		void MoveDown();
};
