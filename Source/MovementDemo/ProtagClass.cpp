// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtagClass.h"
#include <string>

// Sets default values
AProtagClass::AProtagClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProtagCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ProtagCamera"));
	check(ProtagCameraComponent != nullptr);
	ProtagCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	ProtagCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	ProtagCameraComponent->bUsePawnControlRotation = true;

	ProtagMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsComponent"));
	check(ProtagMesh != nullptr);

	ProtagMesh->SetOnlyOwnerSee(true);
	ProtagMesh->SetupAttachment(ProtagCameraComponent);
	ProtagMesh->bCastDynamicShadow = false;
	ProtagMesh->CastShadow = false;
	GetMesh()->SetOwnerNoSee(true);
	ProtagMovement = GetCharacterMovement();

}

// Called when the game starts or when spawned
void AProtagClass::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("playing as protag"));
	
	if (ExpGun) {
		FActorSpawnParameters gun_params;
		gun_params.bNoFail = true;
		gun_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform GunTransform;
		GunTransform.SetLocation(FVector::ZeroVector);
		GunTransform.SetRotation(FQuat(FRotator::ZeroRotator));
		Gun = GetWorld()->SpawnActor<AExplosionGun>(ExpGun, GunTransform, gun_params);
		if (Gun) {
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, TEXT("Got gun"));
		}
	}
	
}

// Called every frame
void AProtagClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector curr_vel = this->GetVelocity();
	std::string log_speeds = "Current speed: " + std::to_string(curr_vel.Size());

	FString curr_speed = FString(log_speeds.c_str());
	std::string cbl = "Can bhop: " + std::to_string(can_bhop);
	FString can_bhop_log = FString(cbl.c_str());
	std::string ifl = "Is falling: " + std::to_string(ProtagMovement->IsFalling());
	FString is_falling_log = FString(ifl.c_str());
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, curr_speed);
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, can_bhop_log);
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, is_falling_log);
	
	if (!can_bhop && !ProtagMovement->IsFalling()) {
		ProtagMovement->MaxWalkSpeed = 600;
	}
}

// Called to bind functionality to input
void AProtagClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AProtagClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProtagClass::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AProtagClass::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AProtagClass::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProtagClass::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AProtagClass::StopJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AProtagClass::Fire);
}

void AProtagClass::MoveForward(float value) {
	FVector direction = GetActorForwardVector();
	AddMovementInput(direction, value);
}

void AProtagClass::MoveRight(float value) {
	FVector direction = GetActorRightVector();
	AddMovementInput(direction, value);
}

void AProtagClass::StartJump() {
	bPressedJump = true;
	if (can_bhop) {
		ProtagMovement->MaxWalkSpeed += 50;
		ProtagMovement->MaxWalkSpeed = FMath::Clamp(ProtagMovement->MaxWalkSpeed, 600.0f, 800.0f);
	}
}

void AProtagClass::ApplyBhop() {
	can_bhop = false;
}

void AProtagClass::StopJump() {
	bPressedJump = false;
	can_bhop = true;
	GetWorldTimerManager().SetTimer(BhopHandler, this, &AProtagClass::ApplyBhop, 0.2f, false, 1.0f);
}

void AProtagClass::Fire() {
	if (Gun) {
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		FRotator MuzzleRotation = CameraRotation;
		CameraRotation.Pitch += 10.0f;

		Gun->Fire(MuzzleLocation, MuzzleRotation);
	}
	/*else if (BulletClass) {
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		FRotator MuzzleRotation = CameraRotation;
		CameraRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			ABullet* SpawnedBullet = World->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (SpawnedBullet) {
				FVector LaunchDirection = MuzzleRotation.Vector();
				SpawnedBullet->FireInDirection(LaunchDirection);
			}
		}
	}
	*/
}