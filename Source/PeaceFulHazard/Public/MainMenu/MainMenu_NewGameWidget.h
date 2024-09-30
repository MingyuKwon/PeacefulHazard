// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PeaceFulHazard/PeaceFulHazard.h"

#include "MainMenu_NewGameWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class AMainMenuGameMode;
class UPeacFulGameInstance;
/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UMainMenu_NewGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BackUIInputTrigger();

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetLangaugeText(ELanguage language);

	UFUNCTION()
	void CheckLanguage();

	void SetDynamicChangeLanguage(UTextBlock* textBlock, const FText& Englishtext, const FText& Koreantext);


	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;




	UPROPERTY(meta = (BindWidget))
	UButton* GameStartButton;

	UFUNCTION()
	void OnGameStartButtonlicked();

	UPROPERTY(meta = (BindWidget))
	UButton* EasyButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NormalButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HardButton;

	UFUNCTION()
	void OnEasyButtonClicked();
	UFUNCTION()
	void OnNormalButtonClicked();
	UFUNCTION()
	void OnHardButtonClicked();

	UFUNCTION()
	void OnEasyButtonHover();
	UFUNCTION()
	void OnNormalButtonHover();
	UFUNCTION()
	void OnHardButtonHover();


	UPROPERTY(meta = (BindWidget))
	UImage* EasyShot;

	UPROPERTY(meta = (BindWidget))
	UImage* NormalShot;

	UPROPERTY(meta = (BindWidget))
	UImage* HardShot;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyExaplainText;


	UFUNCTION()
	void OnLoadButtonClicked();

	UFUNCTION()
	void OnSettingButtonClicked();



	AMainMenuGameMode* MainMenuGameMode;
	UPeacFulGameInstance* PeacFulGameInstance;
};
