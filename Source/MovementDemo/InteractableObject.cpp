// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include <string>

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!MeshComp) {
		MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComp"));
		MeshComp->SetSimulatePhysics(true);
		RootComponent = MeshComp;
	}
	if (!CollisionComponent) {
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		MeshComp->BodyInstance.SetCollisionProfileName(TEXT("Bullet"));
		MeshComp->OnComponentHit.AddDynamic(this, &AInteractableObject::OnHit);
		CollisionComponent->InitSphereRadius(15.0f);
		CollisionComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	ProtagChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	PlayerCamera = ProtagChar->FindComponentByClass<UCameraComponent>();

	TArray<USceneComponent*> Components;
	ProtagChar->GetComponents(Components);
	if (Components.Num() > 0) {
		for (auto& Comp : Components) {
			if (Comp->GetName() == "HoldingComponent") {
				HoldingComp = Cast<USceneComponent>(Comp);
			}
		}
	}
	launched = false;
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	std::string ll = "Is Launched: " + std::to_string(launched);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString(ll.c_str()));
	if (bHolding && HoldingComp) {
		SetActorLocationAndRotation(HoldingComp->GetComponentLocation(), HoldingComp->GetComponentRotation());
	}

}


void AInteractableObject::Pickup() {
	launched = false;
	bHolding = !bHolding;
	MeshComp->SetSimulatePhysics(bHolding ? false : true);
	MeshComp->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
}

void AInteractableObject::Launch() {
	bHolding = !bHolding;
	MeshComp->SetSimulatePhysics(bHolding ? false : true);
	MeshComp->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
	if (!bHolding) {
		ForwardVector = PlayerCamera->GetForwardVector();
		launched = true;
		MeshComp->AddImpulse(ForwardVector * 1000 * MeshComp->GetMass());
	}
}

void AInteractableObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, TEXT("Cube hit not launched"));
	if (launched) {
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, TEXT("Cube hit"));
		if (OtherActor->CanBeDamaged()) {
			FPointDamageEvent ev;
			OtherActor->TakeDamage(Damage, ev, HoldingComp->GetOwner()->GetInstigatorController(), this);
		}
		Destroy();
	}
}