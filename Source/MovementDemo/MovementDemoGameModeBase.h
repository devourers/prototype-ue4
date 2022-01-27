// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MovementDemoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTDEMO_API AMovementDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
		virtual void StartPlay() override;
};
