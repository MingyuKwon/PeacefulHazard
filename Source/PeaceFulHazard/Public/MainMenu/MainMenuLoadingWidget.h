// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuLoadingWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class AMainMenuGameMode;
class UPeacFulGameInstance;
class USoundBase;
class USlider;
class UComboBoxString;

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UMainMenuLoadingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetLoadingOpacity(float Opacity);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonHoverSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonClickSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* BackSound;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadingText;

	UPROPERTY(meta = (BindWidget))
	UBorder* LoadingBackGround;

	AMainMenuGameMode* MainMenuGameMode;

};
