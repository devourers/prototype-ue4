// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAITaskNode.h"
#include "MovementDemo.h"
#include "EnemyAIPatrolController.h"
#include "EnemyPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UEnemyAITaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  AEnemyAIPatrolController* AIContoller = Cast<AEnemyAIPatrolController>(OwnerComp.GetAIOwner());
  if (AIContoller) {
    UBlackboardComponent* BBComp = AIContoller->GetBlackboardComponent();

    AEnemyPatrolPoint* CurrentPoint = Cast<AEnemyPatrolPoint>(BBComp->GetValueAsObject("LocationToGo"));

    TArray<AActor*> AvaliablePoints = AIContoller->GetPatrolPoints();

    AEnemyPatrolPoint* NextPoint = nullptr;

    if (AIContoller->CurrentPatrolPoint != AvaliablePoints.Num() - 1) {
      NextPoint = Cast<AEnemyPatrolPoint>(AvaliablePoints[++AIContoller->CurrentPatrolPoint]);
    }
    else {
      NextPoint = Cast<AEnemyPatrolPoint>(AvaliablePoints[0]);
      AIContoller->CurrentPatrolPoint = 0;
    }

    BBComp->SetValueAsObject("LocationToGo", NextPoint);
    return EBTNodeResult::Succeeded;
  }
  return EBTNodeResult::Failed;
}