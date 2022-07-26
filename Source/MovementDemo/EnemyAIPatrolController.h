// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BasicEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAIPatrolController.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTDEMO_API AEnemyAIPatrolController : public AAIController
{
	GENERATED_BODY()
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UBehaviorTreeComponent* BehTree;
		
	UBlackboardComponent* BBcomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FName LocationToGokey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		FName PlayerKey;

	UPROPERTY()
		ABasicEnemy* Enemy;

	TArray<AActor*> PatrolPoints;
	virtual void OnPossess(APawn* pawn) override;

	int CurrentPatrolPoint;

	AEnemyAIPatrolController();

	void SetPlayerCaught(APawn* pawn);

	bool Shoot();

	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BBcomp; }
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }
	
};
