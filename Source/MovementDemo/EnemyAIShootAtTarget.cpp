// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIShootAtTarget.h"
#include "MovementDemo.h"
#include "EnemyAIPatrolController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProtagClass.h"

EBTNodeResult::Type UEnemyAIShootAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AEnemyAIPatrolController* AIContoller = Cast<AEnemyAIPatrolController>(OwnerComp.GetAIOwner());
	if (!AIContoller) {
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* BBComp = AIContoller->GetBlackboardComponent();

	AProtagClass* Player = Cast<AProtagClass>(BBComp->GetValueAsObject("Target"));

	AIContoller->SetFocus(Player);
	if (BBComp->GetValueAsBool("HasLineOfSight")){
		bool hit_smth = AIContoller->Shoot();

		if (hit_smth) {
			BBComp->SetValueAsBool(FName(TEXT("HitTheTarget")), true);
			return EBTNodeResult::Succeeded;
		}
		else {
			BBComp->SetValueAsBool(FName(TEXT("HitTheTarget")), false);
			return EBTNodeResult::Failed;
		}
	}
	else {
		BBComp->ClearValue("Target");
		AIContoller->ClearFocus(EAIFocusPriority::Gameplay);
		return EBTNodeResult::Failed;
	}
}