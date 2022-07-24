// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProtagClass.h"
#include "MovementDemoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTDEMO_API AMovementDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
		virtual void StartPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Restart() { GetWorld()->ServerTravel("?Restart"); }

public:
	AProtagClass* Player;
};
