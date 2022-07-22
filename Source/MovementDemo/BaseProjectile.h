// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseProjectile.generated.h"

DECLARE_DELEGATE(FOnProjectileHit);

UENUM()
enum ProjectileTypes {
	Explode UMETA(DisplayName = " Exploding "),
	Normal UMETA(DisplayName = " Normal "),
	Rope UMETA(Displayname = " Rope ")
	//TODO
};

UCLASS()
class MOVEMENTDEMO_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Projectile ")
		TEnumAsByte<ProjectileTypes> pType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Damage ")
		float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Damage ")
		float Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Misc ")
		float TraveledDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Damage ")
		float Radius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Projectile ")
		USphereComponent* CollisionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Movement ")
		UProjectileMovementComponent* PrMovementComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Projectile ")
		UStaticMeshComponent* PrMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Movement ")
		UMaterialInstanceDynamic* PrMaterialInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " FX ")
		class UNiagaraSystem* ProjectileEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = " Movement ")
		bool DoesRichochet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AActor* LastHitActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		FVector StartingPosition;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FireInDirection(const FVector& ShootDirection);
	FOnProjectileHit OnProjectileHit;
	UPROPERTY()
		FVector LastLocation;
	UPROPERTY()
		FVector HitLocation;
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
