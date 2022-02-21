// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingingRope.h"
#include "DrawDebugHelpers.h"
#include <string>

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
	//float indexator = RopeComponent->CableLength;
	/*
	for (int i = 0; i < int(indexator / 50); i++) {
		USphereComponent* Current = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
		Current->SetSphereRadius(50.0f);
		Current->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Current->SetRelativeLocation(FVector(i * 50.0f, 0.0f, 0.0f));
		//Current->SetupAttachment(this->RootComponent);
		//Current->AttachTo(RootComponent, TEXT("NAME_None"), EAttachLocation::SnapToTargetIncludingScale, false);
		Current->RegisterComponent();
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, GetTransform().TransformPosition(Current->GetRelativeLocation()).ToString());
		GrabPoints.Add(Current);
	}
	*/
}

// Called every frame
void ASwingingRope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto& Sphere : GrabPoints) {
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, GetTransform().TransformPosition(Sphere->GetRelativeLocation()).ToString());
	}
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
