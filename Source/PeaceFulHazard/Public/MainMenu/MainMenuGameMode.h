// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "MainMenuGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMenuModeChangeEvent, EMainMenuType, menuType, bool, bChangeStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainModeLanguageChangeEvent);

/**
 * 
 */
class UPeacFulGameInstance;
class APostProcessVolume;

UCLASS()
class PEACEFULHAZARD_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	AMainMenuGameMode();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FMainModeLanguageChangeEvent LanguageChangeEvent;

	UFUNCTION()
	void ChangeLanguageCallBack();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FMenuModeChangeEvent MenuModeChangeEvent;

	ELanguage GetCurrentLanguage();
	void SaveSettingValue();
	void LoadSettingValue();

	void SetBGMVolume(float value);
	void SetGameBrightness();
	void GetSettingValue(float& mouse, float& mouseAim);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TMap<EWarpTarget, FString> TravelMap;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TMap<EWarpTarget, USoundBase*> BackgroundMusics;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* BGMAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* UIAudioComponent;


	virtual void BeginPlay() override;

	UPeacFulGameInstance* PeacFulGameInstance;

	APostProcessVolume* CachedPostProcessVolume;

};
