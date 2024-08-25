// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemInformation.h"
#include "DefaultPlayerWidget.generated.h"

class UTextBlock;
class UCanvasPanel;
class UImage;

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UDefaultPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxBulletText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentBulletText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LeftBulletText;

	UPROPERTY(meta = (BindWidget))
	UImage* PistolImage;

	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateBulletUI(int32 currentBullet, int32 maxBullet, int32 leftBullet, EItemType itemType);
};
