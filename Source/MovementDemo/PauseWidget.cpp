// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

bool UPauseWidget::Initialize() {
  Super::Initialize();
  if (ResumeButton){
    ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickResume);
  }
  return true;
}

void UPauseWidget::OnClickResume() {
  UGameplayStatics::SetGamePaused(ResumeButton, UGameplayStatics::IsGamePaused(GetWorld()));
}

void UPauseWidget::ChangeTogglePause() {
  UGameplayStatics::SetGamePaused(GetWorld(), !bTogglePause);
  bTogglePause = !bTogglePause;
}