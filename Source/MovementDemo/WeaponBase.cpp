// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include <string>

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSceneComponent"));
	}
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = WeaponMeshComponent;
	SpawnPoint = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn point"));
	SpawnPoint->SetupAttachment(RootComponent);
	SpawnPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("SpawnSocket"));
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString(std::to_string(wType).c_str()));
	CurrentAmmo = Magazine;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	std::string cal = "Current ammo: " + std::to_string(CurrentAmmo);
	FString current_ammo_log = WeaponName + ", " + FString(cal.c_str());
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, current_ammo_log);
}

void AWeaponBase::AttackWithWeapon(const FVector& MuzzleLocation, const FRotator& MuzzleRotation, AController* Controller) {
	if (!isReloading && CurrentAmmo != 0 && GetWorld()->GetTimeSeconds() > LastShootTime + 1. / FireRate) {
		LastShootTime = GetWorld()->GetTimeSeconds();
		if (SB_shot != nullptr) {
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Playing sound"));
			UGameplayStatics::PlaySoundAtLocation(this, SB_shot, GetActorLocation());
		}
		if (ShotEffect != nullptr) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShotEffect, SpawnPoint->GetComponentLocation(), MuzzleRotation);
		}
		if (wType == EWeaponTypes::eWT_Melee) {
			//TODO
		}
		else if (wType == EWeaponTypes::eWT_RangedProjectile) {
			check(GEngine != nullptr);
			UWorld* World = GetWorld();
			if (World) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();
				Projectile = World->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnPoint->GetComponentLocation(), MuzzleRotation, SpawnParams);
				if (Projectile) {
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
					CurrentAmmo -= 1;
				}
			}
		}
		else if (wType == EWeaponTypes::eWT_RangedHitScan) {
			check(GEngine != nullptr);
			FHitResult ResHit;
			FVector Start = SpawnPoint->GetComponentLocation();
			FVector End = Start + MuzzleRotation.Vector() * Range;
			FCollisionQueryParams c_q_params;
			bool isHit = GetWorld()->LineTraceSingleByChannel(ResHit, Start, End, ECC_MAX, c_q_params);
			if (isHit) {
				if (ResHit.GetActor()->CanBeDamaged()) {
					FPointDamageEvent ev;
					ResHit.GetActor()->TakeDamage(Damage, ev, Controller, this);
				}
				if (HitScanEffect != nullptr) {
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitScanEffect, ResHit.ImpactPoint);
				}
				for (auto cmp : ResHit.GetActor()->GetComponents()) {
					UPrimitiveComponent* cmp_ = Cast<UPrimitiveComponent>(cmp);
					if (cmp_ && cmp_->IsSimulatingPhysics()) {
						cmp_->AddImpulseAtLocation(MuzzleRotation.Vector() * 10.0f * Range, ResHit.ImpactPoint);
					}
				}
				//if (ResHit.GetComponent()->IsSimulatingPhysics()){
			  //	ResHit.GetComponent()->AddImpulseAtLocation(MuzzleRotation.Vector() * 10.0f * Range, ResHit.ImpactPoint);
				//}
			}
			CurrentAmmo -= 1;
		}
	}
	else {
		//todo
	}
}

void AWeaponBase::StopReload() {
	isReloading = false;
}

void AWeaponBase::Reload() {
	if (!isReloading && AmmoCount > 0) {
		isReloading = true;
		if (AmmoCount > Magazine){
			AmmoCount -= Magazine - CurrentAmmo;
			CurrentAmmo = Magazine;
		}
		else {
			CurrentAmmo = AmmoCount;
			AmmoCount = 0;
		}
		//play animation;
		GetWorldTimerManager().SetTimer(ReloadHandler, this, &AWeaponBase::StopReload, 0.2f, false, 1.0f);
	}
}

void AWeaponBase::HideWeapon(bool to_hide) {
	SetActorHiddenInGame(to_hide);
}