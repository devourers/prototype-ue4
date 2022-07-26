// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIPatrolController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyPatrolPoint.h"
#include "ProtagClass.h"



AEnemyAIPatrolController::AEnemyAIPatrolController() {
  BehTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehComp"));
  BBcomp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
  PlayerKey = "Target";
  LocationToGokey = "LocationToGo";
  CurrentPatrolPoint = 0;
}

void AEnemyAIPatrolController::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  if (BBcomp) {
    if (BBcomp->GetValueAsObject(PlayerKey)) {
      if (!LineOfSightTo(Cast<AActor>(BBcomp->GetValueAsObject(PlayerKey)))) {
        BBcomp->SetValueAsBool("HasLineOfSight", false);
        BBcomp->ClearValue(PlayerKey);
      }
    }
  }
}

void AEnemyAIPatrolController::SetPlayerCaught(APawn* pawn) {
  if (BBcomp) {
    BBcomp->SetValueAsObject(PlayerKey, pawn);
    SetFocus(pawn);
    if (LineOfSightTo(pawn)) {
      BBcomp->SetValueAsBool("HasLineOfSight", true);
    }
    else {
      BBcomp->SetValueAsBool("HasLineOfSight", false);
    }
  }
}

void AEnemyAIPatrolController::OnPossess(APawn* pawn) {
  Super::OnPossess(pawn);

  Enemy = Cast<ABasicEnemy>(pawn);

  if (Enemy) {
    if (Enemy->BTree->BlackboardAsset) {
      BBcomp->InitializeBlackboard(*(Enemy->BTree->BlackboardAsset));
    }

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyPatrolPoint::StaticClass(), PatrolPoints);

    BehTree->StartTree(*Enemy->BTree);
  }
}

bool AEnemyAIPatrolController::Shoot() {
  bool is_enemy_hit = false;
  if (Enemy && Enemy->Weapon && Enemy->CurrentHealth > 0) {
    AWeaponBase* weapon = Cast<AWeaponBase>(Enemy->Weapon->GetChildActor());
    if (weapon && weapon->CurrentAmmo > 0){
      GetControlRotation();
      FRotator a;
      a.Roll = FMath::RandRange(-10.0f, 10.0f); //TODO gun deviation in params
      a.Yaw = FMath::RandRange(-10.0f, 10.0f); //TODO gun deviation in params
      is_enemy_hit = weapon->AttackWithWeapon(FVector(0), GetControlRotation() + a, this);
    }
    else if (weapon->CurrentAmmo == 0) {
      weapon->Reload();
    }
  }
  return is_enemy_hit;
}