// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIPatrolController.h"
#include "BasicEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyPatrolPoint.h"



AEnemyAIPatrolController::AEnemyAIPatrolController() {
  BehTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehComp"));
  BBcomp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
  PlayerKey = "Target";
  LocationToGokey = "LocationToGo";
  CurrentPatrolPoint = 0;
}

void AEnemyAIPatrolController::SetPlayerCaught(APawn* pawn) {
  if (BBcomp) {
    BBcomp->SetValueAsObject(PlayerKey, pawn);
  }
}

void AEnemyAIPatrolController::OnPossess(APawn* pawn) {
  Super::OnPossess(pawn);

  ABasicEnemy* Enemy = Cast<ABasicEnemy>(pawn);

  if (Enemy) {
    if (Enemy->BTree->BlackboardAsset) {
      BBcomp->InitializeBlackboard(*(Enemy->BTree->BlackboardAsset));
    }

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyPatrolPoint::StaticClass(), PatrolPoints);

    BehTree->StartTree(*Enemy->BTree);
  }
}