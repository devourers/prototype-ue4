// Copyright Epic Games, Inc. All Rights Reserved.


#include "MovementDemoGameModeBase.h"

void AMovementDemoGameModeBase::StartPlay() {
  Super::StartPlay();
  check(GEngine != nullptr);
  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("penis"));
}
