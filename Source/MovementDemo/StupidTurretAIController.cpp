// Fill out your copyright notice in the Description page of Project Settings.


#include "StupidTurretAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ProtagClass.h"
#include "BasicEnemy.h"
#include "DrawDebugHelpers.h"

void AStupidTurretAIController::Tick(float DeltaTime)
{
	AAIController::Tick(DeltaTime);
	// WHY?????	
	//Super().Tick(DeltaTime);
	
	APawn* pawn = GetPawn();
	if (!pawn) return;

	if (!GetWorld()) return;

	AActor *target = UGameplayStatics::GetActorOfClass(GetWorld(), AProtagClass::StaticClass());

	if (!target) return;
	SetFocus(target);

	cnt++;

	if (projectileCls && (cnt % 10 == 0)) {
		ABaseProjectile *proj = GetWorld()->SpawnActor<ABaseProjectile>(projectileCls, pawn->GetActorLocation() + FVector(0, 0, 300), FRotator());
		proj->FireInDirection((target->GetActorLocation() - proj->GetActorLocation()).GetUnsafeNormal());
	}
}
