// Copyright Epic Games, Inc. All Rights Reserved.


#include "MovementDemoGameModeBase.h"

void AMovementDemoGameModeBase::StartPlay() {
  Super::StartPlay();
  check(GEngine != nullptr);
}
