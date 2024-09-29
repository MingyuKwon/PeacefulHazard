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

class AMainMenuGameMode;
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


	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NewGaemButton;



	UFUNCTION()
	void OnLoadButtonClicked();

	UFUNCTION()
	void OnNewGameButtonClicked();


	AMainMenuGameMode* MainMenuGameMode;
};
