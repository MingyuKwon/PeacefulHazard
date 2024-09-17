// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AimCrossHairWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UAimCrossHairWidget::setAimWide(float value)
{
	FVector2D PositionSetAim[4] = {
		FVector2D(0.f, value * 25),   // Down
		FVector2D(value * 25, 0.f),    // Right
		FVector2D(-value * 25, 0.f),   // Left
		FVector2D(0.f, - value * 25)     // Up
	};

	UCanvasPanelSlot* CrossHairSlot = nullptr;

	CrossHairSlot = Cast<UCanvasPanelSlot>(PistolCrossHairDown->Slot);
	if (CrossHairSlot) CrossHairSlot->SetPosition(PositionSetAim[0]);

	CrossHairSlot = Cast<UCanvasPanelSlot>(PistolCrossHairRight->Slot);
	if (CrossHairSlot) CrossHairSlot->SetPosition(PositionSetAim[1]);
	
	CrossHairSlot = Cast<UCanvasPanelSlot>(PistolCrossHairLeft->Slot);
	if (CrossHairSlot) CrossHairSlot->SetPosition(PositionSetAim[2]);

	CrossHairSlot = Cast<UCanvasPanelSlot>(PistolCrossHairUp->Slot);
	if (CrossHairSlot) CrossHairSlot->SetPosition(PositionSetAim[3]);



}
