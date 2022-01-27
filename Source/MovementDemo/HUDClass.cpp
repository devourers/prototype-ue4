// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDClass.h"

void AHUDClass::DrawHUD() {
  Super::DrawHUD();

  if (CrosshairTexture) {
    FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

    FVector2D CrosshairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

    FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
    TileItem.BlendMode = SE_BLEND_Translucent;
    Canvas->DrawItem(TileItem);
  }
}