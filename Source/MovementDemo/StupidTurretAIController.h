// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseProjectile.h"
#include "StupidTurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTDEMO_API AStupidTurretAIController : public AAIController
{
	GENERATED_BODY()

		AStupidTurretAIController() { PrimaryActorTick.bCanEverTick = true;  }

	virtual void Tick(float DeltaTime) override;

public:
	// Wtf, this should be inside enemy
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseProjectile> projectileCls;

	int cnt = 0;
};
