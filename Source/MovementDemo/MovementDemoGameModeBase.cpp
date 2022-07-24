// Copyright Epic Games, Inc. All Rights Reserved.


#include "MovementDemoGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AMovementDemoGameModeBase::StartPlay() {
  Super::StartPlay();
  check(GEngine != nullptr);
  TArray<AActor*> protag_arr;
  Player = Cast<AProtagClass>(UGameplayStatics::GetActorOfClass(GetWorld(), AProtagClass::StaticClass()));
}

void AMovementDemoGameModeBase::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  if (Player && Player->Health == 0) {
	  FTimerHandle dummy;
	  GetWorldTimerManager().SetTimer(dummy, this, &AMovementDemoGameModeBase::Restart, 1.0f);
  }
}
