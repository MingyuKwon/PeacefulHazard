// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "MainMenuHUD.generated.h"

class UMainMenu_NewGameWidget;
class UMainMenu_SettingWidget;
class UMainMenu_LoadWidget;
class UMainMenu_DefaultWidget;
class UMainMenuLoadingWidget;

class UCheckOnceMoreWidget;

class AMainMenuGameMode;
/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void BackUIInputTrigger();

	void OneMoreCheckBackUIInputTrigger();
	void OneMoreCheckOkUIInputTrigger();
	bool GetOneMoreCheckVisible();

	UFUNCTION()
	void SetOnceCheckDisplay(bool bVisible, const FText EnglishText = FText(), const FText KoreanText = FText());


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainMenu_NewGameWidget> MainMenu_NewGameWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainMenu_SettingWidget> MainMenu_SettingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainMenu_LoadWidget> MainMenu_LoadWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainMenu_DefaultWidget> MainMenu_DefaultWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainMenuLoadingWidget> MainMenuLoadingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCheckOnceMoreWidget> CheckOnceMoreWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* BackUISound;

	virtual void BeginPlay() override;

	AMainMenuGameMode* MainMenuGameMode;

	UFUNCTION()
	void ChangeMenuMode(EMainMenuType menuType, bool bChangeStart);

	void ChangeShowMenuUI(EMainMenuType menuType);

	UPROPERTY()
	UMainMenu_NewGameWidget* MainMenu_NewGameWidget;

	UPROPERTY()
	UMainMenu_SettingWidget* MainMenu_SettingWidget;

	UPROPERTY()
	UMainMenu_LoadWidget* MainMenu_LoadWidget;

	UPROPERTY()
	UMainMenu_DefaultWidget* MainMenu_DefaultWidget;

	UPROPERTY()
	UMainMenuLoadingWidget* MainMenuLoadingWidget;

	UCheckOnceMoreWidget* CheckOnceMoreWidget;

};
