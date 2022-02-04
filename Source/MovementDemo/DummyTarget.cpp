// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyTarget.h"
#include <string>

// Sets default values
ADummyTarget::ADummyTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DummySceneComponent"));
	}
	if (!DummyMesh) {
		DummyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
		check(DummyMesh != nullptr);
	}
}

// Called when the game starts or when spawned
void ADummyTarget::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

// Called every frame
void ADummyTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
	if (CurrentHealth <= 0.0) {
		DummyMesh->SetAllBodiesSimulatePhysics(true);
		DummyMesh->SetSimulatePhysics(true);
		DummyMesh->WakeAllRigidBodies();
	}

}

float ADummyTarget::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.0) {
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, TEXT("Now I am dead."));
		DummyMesh->SetAllBodiesSimulatePhysics(true);
		DummyMesh->SetSimulatePhysics(true);
		DummyMesh->WakeAllRigidBodies();
	}
	return DamageAmount;
}