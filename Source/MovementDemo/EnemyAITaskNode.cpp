// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAITaskNode.h"
#include "MovementDemo.h"
#include "EnemyAIPatrolController.h"
#include "EnemyPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UEnemyAITaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AEnemyAIPatrolController* AIContoller = Cast<AEnemyAIPatrolController>(OwnerComp.GetAIOwner());
	if (!AIContoller) {
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* BBComp = AIContoller->GetBlackboardComponent();

	AEnemyPatrolPoint* CurrentPoint = Cast<AEnemyPatrolPoint>(BBComp->GetValueAsObject("LocationToGo"));

	TArray<AActor*> AvaliablePoints = AIContoller->GetPatrolPoints();

	if (AvaliablePoints.Num() == 0) {
		return EBTNodeResult::Failed;
	}

	AIContoller->CurrentPatrolPoint = (AIContoller->CurrentPatrolPoint + 1) % AvaliablePoints.Num();

	AEnemyPatrolPoint* NextPoint = Cast<AEnemyPatrolPoint>(AvaliablePoints[AIContoller->CurrentPatrolPoint]);

	BBComp->SetValueAsObject("LocationToGo", NextPoint);
	return EBTNodeResult::Succeeded;
}