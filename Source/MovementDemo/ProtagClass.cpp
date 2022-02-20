// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtagClass.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include <string>

// Sets default values
AProtagClass::AProtagClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProtagCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ProtagCamera"));
	check(ProtagCameraComponent != nullptr);
	ProtagCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	ProtagCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	ProtagCameraComponent->bUsePawnControlRotation = true;
	ProtagMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsComponent"));
	check(ProtagMesh != nullptr);

	ProtagMesh->SetOnlyOwnerSee(true);
	ProtagMesh->SetupAttachment(ProtagCameraComponent);
	ProtagMesh->bCastDynamicShadow = false;
	ProtagMesh->CastShadow = false;
	GetMesh()->SetOwnerNoSee(true);
	ProtagMovement = GetCharacterMovement();

	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	HoldingComponent->SetupAttachment(ProtagMesh);

	CurrentItem = NULL;

}

// Called when the game starts or when spawned
void AProtagClass::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("playing as protag"));
	for (int i = 0; i < WeaponInventoryClasess.Num(); i++) {
		FActorSpawnParameters gun_params;
		gun_params.bNoFail = true;
		gun_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform GunTransform;
		GunTransform.SetLocation(FVector::ZeroVector);
		GunTransform.SetRotation(FQuat(FRotator::ZeroRotator));
		class AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponInventoryClasess[i], GunTransform, gun_params);
		WeaponInventory.Add(Weapon);
		Weapon->SetOwner(this);
		Weapon->AttachToComponent(ProtagMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("b_RightWeaponSocket"));
		if (i != current_weapon) {
			Weapon->HideWeapon(true);
		}
	}
}

// Called every frame
void AProtagClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetCameraPos();

	FVector curr_vel = this->GetVelocity();
	std::string log_speeds = "Current speed: " + std::to_string(curr_vel.Size());
	FString curr_speed = FString(log_speeds.c_str());

	std::string cbl = "Can bhop: " + std::to_string(can_bhop);
	FString can_bhop_log = FString(cbl.c_str());

	std::string ifl = "Is falling: " + std::to_string(ProtagMovement->IsFalling());
	FString is_falling_log = FString(ifl.c_str());

	std::string cwl = "Current weapon: ";
	FString current_weapon_log = FString(cwl.c_str()) + WeaponInventory[current_weapon]->WeaponName;

	std::string al = "Ammo left: " + std::to_string(WeaponInventory[current_weapon]->AmmoCount);
	FString ammo_log = FString(al.c_str());

	std::string cswl = "Can switch weapons: " + std::to_string(can_switch);
	FString can_switch_weapons_log = FString(cswl.c_str());

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, curr_speed);
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, current_weapon_log);
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, ammo_log);
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, can_switch_weapons_log);


	if (!can_bhop && !ProtagMovement->IsFalling() && !is_sprinting) {
		ProtagMovement->MaxWalkSpeed = 600;
	}

	InteractStart = ProtagCameraComponent->GetComponentLocation();
	ForwardVector = ProtagCameraComponent->GetForwardVector();
	InteractFinish = ((ForwardVector * 500.0f) + InteractStart);

	if (!isHolding) {
		if (GetWorld()->LineTraceSingleByChannel(InteractHit, InteractStart, InteractFinish, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam)) {
			if (InteractHit.GetActor()->GetClass()->IsChildOf(AInteractableObject::StaticClass())) {
				CurrentItem = Cast<AInteractableObject>(InteractHit.GetActor());
			}
		}
		else {
			CurrentItem = NULL;
		}
	}
}

void AProtagClass::SetCameraPos() {
	float collisionCenterZ = GetCapsuleComponent()->GetRelativeLocation().Z;
	float height = collisionCenterZ + BaseEyeHeight;
	float leanRad = FMath::DegreesToRadians(MaxLeanAngle) * LeanAmount;
	FQuat leanRot = FQuat(FVector(1, 0, 0), -leanRad);
	FVector eyePos = FVector(0, 0, height * (1 - LeanHeightRatio)) + leanRot.RotateVector(FVector(0, 0, height * LeanHeightRatio));
	ProtagCameraComponent->SetRelativeLocation(eyePos - FVector(0, 0, collisionCenterZ));
	ProtagCameraComponent->SetWorldRotation(FRotator(GetControlRotation().Quaternion() * leanRot));
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
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AProtagClass::Reload);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AProtagClass::SwitchWeapons);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AProtagClass::Interact);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AProtagClass::ToggleCrouch);
	PlayerInputComponent->BindAxis("Lean", this, &AProtagClass::Lean);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AProtagClass::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AProtagClass::StopSprint);
}


void AProtagClass::AddControllerPitchYawInput(float pitch, float yaw) {
	float angle = MaxLeanAngle * LeanAmount;
	FVector2D fixed = FVector2D(pitch, yaw).GetRotated(-angle);
	Super::AddControllerYawInput(fixed.Y);
	Super::AddControllerPitchInput(fixed.X);
}

void AProtagClass::AddControllerYawInput(float Val) {
	AddControllerPitchYawInput(0, Val);
}

void AProtagClass::AddControllerPitchInput(float Val) {
	AddControllerPitchYawInput(Val, 0);
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
		ProtagMovement->MaxWalkSpeed = FMath::Clamp(ProtagMovement->MaxWalkSpeed, 600.0f, 1000.0f);
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
	if (!isHolding && !is_sprinting){
		if (WeaponInventory[current_weapon] && WeaponInventory[current_weapon]->CurrentAmmo != 0) {
			FVector CameraLocation;
			FRotator CameraRotation;
			GetActorEyesViewPoint(CameraLocation, CameraRotation);
			MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

			FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

			FRotator MuzzleRotation = CameraRotation;
			CameraRotation.Pitch += 10.0f;
			WeaponInventory[current_weapon]->AttackWithWeapon(MuzzleLocation, MuzzleRotation, Controller);
		}
	}
	else if (isHolding && !is_sprinting){
		CurrentItem->Launch();
		isHolding = false;
		CurrentItem = NULL;
		ProtagMesh->SetOwnerNoSee(false);
		can_switch = true;
		WeaponInventory[current_weapon]->WeaponMeshComponent->SetOwnerNoSee(false);

	}
}

void AProtagClass::Reload() {

	if (WeaponInventory[current_weapon]->CurrentAmmo != WeaponInventory[current_weapon]->Magazine && WeaponInventory[current_weapon]->AmmoCount > 0){
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("I Am Reloading"));
		WeaponInventory[current_weapon]->Reload();
	}
}


void AProtagClass::SwitchWeapons() {
	//TODO -- hide models
	if (can_switch){
		APlayerController* inp = GetWorld()->GetFirstPlayerController();
		if (inp->WasInputKeyJustPressed(EKeys::One) && current_weapon != 0) {
			can_switch = false;
			WeaponInventory[1]->HideWeapon(true);
			current_weapon = 0;
			WeaponInventory[0]->HideWeapon(false);
			GetWorldTimerManager().SetTimer(SwitchHandler, this, &AProtagClass::CanSwitchAgain, 0.2f, false, 1.0f);
		}
		if (inp->WasInputKeyJustPressed(EKeys::Two) && current_weapon != 1) {
			can_switch = false;
			WeaponInventory[0]->HideWeapon(true);
			current_weapon = 1;
			WeaponInventory[1]->HideWeapon(false);
			GetWorldTimerManager().SetTimer(SwitchHandler, this, &AProtagClass::CanSwitchAgain, 0.2f, false, 1.0f);
		}
	}
}

void AProtagClass::CanSwitchAgain() {
	can_switch = true;
}

void AProtagClass::Interact() {
	if (CurrentItem && !isHolding) {
		ProtagMesh->SetOwnerNoSee(true);
		WeaponInventory[current_weapon]->WeaponMeshComponent->SetOwnerNoSee(true);
		can_switch = false;
		ToggleItemPick();
	}
	else if (CurrentItem && isHolding) {
		ProtagMesh->SetOwnerNoSee(false);
		WeaponInventory[current_weapon]->WeaponMeshComponent->SetOwnerNoSee(false);
		can_switch = true;
		ToggleItemPick();
	}
}

void AProtagClass::ToggleItemPick() {
	if (CurrentItem)
	{
		isHolding = !isHolding;
		CurrentItem->Pickup();
		if (!isHolding) {
			CurrentItem = NULL;
		}
	}
}

void AProtagClass::ToggleCrouch() {
	
	if (!is_crouching) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Crouching"));
		this->Crouch(true);
		is_crouching = true;
	}
	else if (is_crouching) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("UnCrouching"));
		this->UnCrouch(true);
		is_crouching = false;
	}
}

void AProtagClass::Lean(float d) {
	LeanAmount = d;
}

void AProtagClass::Sprint() {
	if (!is_sprinting){
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Started Sprinting"));
		ProtagMovement->MaxWalkSpeed += 200;
		is_sprinting = true;
	}
}

void AProtagClass::StopSprint() {
	if (is_sprinting) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Stopped Sprinting"));
		ProtagMovement->MaxWalkSpeed -= 200;
		is_sprinting = false;
	}
}