// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "CheckOnceMoreWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class APeaceFulHazardGameMode;
class AMainMenuGameMode;

class UPeacFulGameInstance;
class USoundBase;
class USlider;
class UComboBoxString;

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UCheckOnceMoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BackUIInputTrigger();
	void OkUIInputTrigger();

	void SetOneMoreDisplay(bool bVisible, const FText EnglishText = FText(), const FText KoreanText = FText());

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CheckOnceText;

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	AMainMenuGameMode* MainMenuGameMode;
	UPeacFulGameInstance* PeacFulGameInstance;


	UFUNCTION(BlueprintImplementableEvent)
	void SetLangaugeText(ELanguage language);


	UFUNCTION()
	void CheckLanguage();

};
