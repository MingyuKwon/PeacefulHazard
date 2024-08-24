// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CanInteractWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UCanInteractWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* ActionImage;

	UPROPERTY(meta = (BindWidget))
	UImage* InteractImage;

	UPROPERTY(meta = (BindWidget))
	UImage* KeyImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KeyNameText;


public:
	void SetActionEnable(bool bCanShowUI, bool flag);

	void SetInteractEnable(bool bCanShowUI, bool flag, UTexture2D* DoorInteractkeyImage, FString DoorInteractkeyName);
};
