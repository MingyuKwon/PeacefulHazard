// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AimCrossHairWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UAimCrossHairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void setAimWide(float value);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* PistolCrossHairDown;

	UPROPERTY(meta = (BindWidget))
	UImage* PistolCrossHairLeft;

	UPROPERTY(meta = (BindWidget))
	UImage* PistolCrossHairRight;

	UPROPERTY(meta = (BindWidget))
	UImage* PistolCrossHairUp;

};
