// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_DefaultWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class AMainMenuGameMode;
/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UMainMenu_DefaultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BackUIInputTrigger();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonHoverSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonClickSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* BackSound;


	UFUNCTION(BlueprintImplementableEvent)
	void SetLangaugeText(ELanguage language);

	UFUNCTION()
	void CheckLanguage();


	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NewGaemButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;


	UFUNCTION()
	void OnQuitButtonClicked();

	UFUNCTION()
	void OnLoadButtonClicked();

	UFUNCTION()
	void OnSettingButtonClicked();

	UFUNCTION()
	void OnNewGameButtonClicked();

	UFUNCTION()
	void OnQuitButtonHovered();

	UFUNCTION()
	void OnLoadButtonHovered();

	UFUNCTION()
	void OnSettingButtonHovered();

	UFUNCTION()
	void OnNewGameButtonHovered();



	AMainMenuGameMode* MainMenuGameMode;
};
