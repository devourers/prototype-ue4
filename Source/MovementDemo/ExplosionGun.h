// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplodingProjectile.h"
#include "ExplosionGun.generated.h"

UCLASS()
class MOVEMENTDEMO_API AExplosionGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExplosionGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AExplodingProjectile> ExpProjectile;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
		USkeletalMeshComponent* ExGunMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
		FColor GunColor;
	UFUNCTION()
		void Fire(const FVector& MuzzleLocation, const FRotator& MuzzleRotation);

};