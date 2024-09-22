// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "Controller/HappyPlayerController.h"
#include "System/PeacFulSaveGame.h"
#include "PeacFulGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInstanceLanguageChangeEvent);

class UPeacFulSaveGame;
class USoundBase;
class UAudioComponent;
class USettingSave;
/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UPeacFulGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FInstanceLanguageChangeEvent LanguageChangeEvent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TMap<ETutorialType, bool> TutorialCheckMap;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TArray<EPlayerToDo> currentToDos;
	int32 todoIndex = 0;


	TMap<EPlayerToDo, FString> GetToDoMapByLanguage();
	TMap<EWarpTarget, FString> GetMapNameByLanguage();
	TMap<ETutorialType, FString> GetTutorialByLanguage();

	EDifficulty gameDifficulty = EDifficulty::ED_Normal;

	int32 currentEnemyForce = 0;

	void UpdateToDo(EPlayerToDo targetTodo);

	EPlayerToDo GetCurrentToDo(FString& TodoString);

	bool checkIsTutorialAlready(ETutorialType tutorial);

	void RefreshGame();

	UPROPERTY();
	UPeacFulSaveGame* tempSaveGame;
	FString SelectedSaveSlot = FString("");

	void resetTempSave();


	void PlaySoundOnceInGamePlay(USoundBase* Sound, FVector Location, float VolumeScale);
	void PlayAudioComponent(EGameSoundType soundType, UAudioComponent* AudioComponent, USoundBase* Sound, float VolumeScale);


	//////////////////////////////////////// SETTING /////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseSensitivity = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseAimSensitivity = 0.4f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Parameter")
	float SFXVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Parameter")
	float BGMVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Parameter")
	float UIVolume = 0.5f;

	void ResetSetting();

	void LoadSettingValue();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	float Brightness = 14.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	FString Resolution;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	FString Language;


	void SetResolution(FString SelectedItem);

	void SetLangauage(FString SelectedItem);

	//////////////////////////////////////// SETTING /////////////////////////////////////////////////////////////////////

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EPlayerToDo, FString> ToDoMapEnglish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EPlayerToDo, FString> ToDoMapKorean;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TMap<EWarpTarget, FString> MapNameEnglish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TMap<EWarpTarget, FString> MapNameKorean;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TMap<ETutorialType, FString> TutorialMapEnglish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TMap<ETutorialType, FString> TutorialMapKorean;


	virtual void Init() override;

	bool bBeforeWasMap = false;



};

