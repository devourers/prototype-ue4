// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingingRope.h"

// Sets default values
ASwingingRope::ASwingingRope()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RopeComponent = CreateDefaultSubobject<UCableComponent>(TEXT("RopeComponent"));
	RopeComponent->CableLength = 1000.0f;
	CurrentGrabPlace = CreateDefaultSubobject<USceneComponent>(TEXT("GrabbingComponent"));
	RootComponent = RopeComponent;
	CurrentGrabPlace->SetupAttachment(RopeComponent);
}

// Called when the game starts or when spawned
void ASwingingRope::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwingingRope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwingingRope::GetGrabbed(const USphereComponent* GrabbedSphere) {
	CurrentGrabPlace->SetRelativeLocation(GrabbedSphere->GetRelativeLocation());
}

void ASwingingRope::MoveUp() {
	FVector CurrLocation = CurrentGrabPlace->GetRelativeLocation();
	if (CurrLocation.X <= RopeComponent->CableLength) {
		CurrentGrabPlace->SetRelativeLocation(FVector(CurrLocation.X + 10.0, CurrLocation.Y, CurrLocation.Z));
	}
}

void ASwingingRope::MoveDown() {
	FVector CurrLocation = CurrentGrabPlace->GetRelativeLocation();
	if (CurrLocation.X >= 10.0){
		CurrentGrabPlace->SetRelativeLocation(FVector(CurrLocation.X - 10.0, CurrLocation.Y, CurrLocation.Z));
	}
}
