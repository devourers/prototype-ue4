// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTDEMO_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		virtual bool Initialize() override;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class UButton* ResumeButton;

		UFUNCTION()
	void OnClickResume();
		UFUNCTION()
	void ChangeTogglePause();


		bool bTogglePause = false;
};
