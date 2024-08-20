// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CanInteractWidget.generated.h"

class UImage;
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

public:
	void SetActionEnable(bool flag);

	void SetInteractEnable(bool flag);

};
