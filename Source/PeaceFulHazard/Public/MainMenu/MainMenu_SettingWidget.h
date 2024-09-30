// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_SettingWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class UComboBoxString;
class USlider;

class AMainMenuGameMode;
class UPeacFulGameInstance;

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UMainMenu_SettingWidget : public UUserWidget
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


	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NewGaemButton;

	UFUNCTION()
	void OnLoadButtonClicked();

	UFUNCTION()
	void OnNewGameButtonClicked();

	UFUNCTION()
	void OnLoadButtonHovered();

	UFUNCTION()
	void OnNewGameButtonHoverd();


	AMainMenuGameMode* MainMenuGameMode;
	UPeacFulGameInstance* PeacFulGameInstance;

	/////////////////////// option panel ////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent)
	void SetLangaugeText(ELanguage language);

	UFUNCTION()
	void CheckLanguage();


	UPROPERTY(meta = (BindWidget))
	UButton* SettingResetButton;


	UPROPERTY(meta = (BindWidget))
	USlider* DefaultMouseSensibility;

	UPROPERTY(meta = (BindWidget))
	UBorder* DefaultMouseSensibilityFill;

	float maxDefaultMouseSensibilityFill = 1000;

	UPROPERTY(meta = (BindWidget))
	USlider* AimMouseSensibility;

	UPROPERTY(meta = (BindWidget))
	UBorder* AimMouseSensibilityFill;

	float maxAimMouseSensibilityFill = 1000;



	UPROPERTY(meta = (BindWidget))
	USlider* BGMVolumue;

	UPROPERTY(meta = (BindWidget))
	UBorder* BGMVolumueFill;

	UPROPERTY(meta = (BindWidget))
	USlider* SFXVolumue;

	UPROPERTY(meta = (BindWidget))
	UBorder* SFXVolumueFill;

	UPROPERTY(meta = (BindWidget))
	USlider* UIVolumue;

	UPROPERTY(meta = (BindWidget))
	UBorder* UIVolumueFill;

	float maxVolumeFill = 500.0f;


	UPROPERTY(meta = (BindWidget))
	USlider* Brightness;

	UPROPERTY(meta = (BindWidget))
	UBorder* BrightnessFill;

	float maxBrightnessFill = 440.0f;



	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResolutionDropDown;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* LanguageDropDown;



	UFUNCTION()
	void OnResolutionDropDownChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnLanguageDropDownChanged(FString SelectedItem, ESelectInfo::Type SelectionType);


	UFUNCTION()
	void OnDefaultMouseSensibilityValueChanged(float Value);

	UFUNCTION()
	void OnAimMouseSensibilityValueChanged(float Value);


	UFUNCTION()
	void OnBGMChanged(float Value);

	UFUNCTION()
	void OnSFXChanged(float Value);

	UFUNCTION()
	void OnUIChanged(float Value);

	UFUNCTION()
	void OnBrightChanged(float Value);

	UFUNCTION()
	void OnResetSettingValue();

	UFUNCTION(BlueprintCallable)
	void UpdateAllUI();


	/////////////////////// option panel ////////////////////////////////////////////




};
