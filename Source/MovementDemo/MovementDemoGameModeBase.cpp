// Copyright Epic Games, Inc. All Rights Reserved.


#include "MovementDemoGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AMovementDemoGameModeBase::StartPlay() {
  Super::StartPlay();
  check(GEngine != nullptr);
  TArray<AActor*> protag_arr;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProtagClass::StaticClass(), protag_arr);
  Player = Cast<AProtagClass>(protag_arr[0]);
}

void AMovementDemoGameModeBase::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  if (Player && Player->Health == 0) {
	  Restart();
  }
}
