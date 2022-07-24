// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAIPatrolController.h"
#include "Perception/PawnSensingComponent.h"
#include "ProtagClass.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABasicEnemy::ABasicEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(90.0f);
	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("hand_l")); // can crash if no mesh todo
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp) {
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ABasicEnemy::OnPlayerCaught);
	}
	CurrentHealth = MaxHealth;
}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
	if (CurrentHealth <= 0.0) {
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetWorldTimerManager().SetTimer(DeathHandler, this, &ABasicEnemy::Die, 0.2f, false, 1.0f);
		UnPossessed();
	}
	
	// TODO: move this logic to AIController
	AEnemyAIPatrolController* ai = Cast<AEnemyAIPatrolController>(GetController());
	AActor *target = UGameplayStatics::GetActorOfClass(GetWorld(), AProtagClass::StaticClass());
	AWeaponBase* weapon = Cast<AWeaponBase>(Weapon->GetChildActor());

	if (ai && target && weapon) {
		ai->SetFocus(target);
		FHitResult ResHit;
		FVector Start = weapon->SpawnPoint->GetComponentLocation();
		FVector End = Start + GetControlRotation().Vector() * weapon->Range;
		FCollisionQueryParams c_q_params;
		bool isHit = GetWorld()->LineTraceSingleByChannel(ResHit, Start, End, ECC_MAX, c_q_params);
		FRotator a;
		a.Roll = FMath::RandRange(-10.0f, 10.0f); //TODO gun deviation in params
		a.Yaw = FMath::RandRange(-10.0f, 10.0f); //TODO gun deviation in params
		if (ResHit.GetActor() == target && weapon->CurrentAmmo)
			weapon->AttackWithWeapon(FVector(0), GetControlRotation() + a, ai);
		else
			weapon->Reload();
	}
}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABasicEnemy::OnPlayerCaught(APawn* pawn) {
	AEnemyAIPatrolController* AIController = Cast<AEnemyAIPatrolController>(GetController());
	if (AIController) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("Popavs"));
		AIController->SetPlayerCaught(pawn);
	}
}

float ABasicEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	if (CurrentHealth > 0.0) {
		CurrentHealth -= DamageAmount;
		if (CurrentHealth <= 0.0) {
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetSimulatePhysics(true);
			GetMesh()->WakeAllRigidBodies();
			GetWorldTimerManager().SetTimer(DeathHandler, this, &ABasicEnemy::Die, 0.2f, false, 1.0f);
		}
	}
	return DamageAmount;
}

void ABasicEnemy::Die() {
	
}