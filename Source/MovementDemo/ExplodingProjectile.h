// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "ExplodingProjectile.generated.h"

class USoundBase;

UCLASS()
class MOVEMENTDEMO_API AExplodingProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplodingProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& ShootDirection);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile)
		USphereComponent* CollisionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
		UProjectileMovementComponent* ExProjectileMovementComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile)
		UStaticMeshComponent* ProjectileMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Material")
		UMaterialInstanceDynamic* ProjectileMaterialInstance;


	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
