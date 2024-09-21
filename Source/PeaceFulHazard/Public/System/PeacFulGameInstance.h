// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "Controller/HappyPlayerController.h"
#include "System/PeacFulSaveGame.h"
#include "PeacFulGameInstance.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<ETutorialType, bool> TutorialCheckMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TMap<ETutorialType, FString> TutorialMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TArray<EPlayerToDo> currentToDos;
	int32 todoIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EPlayerToDo, FString> ToDoMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EWarpTarget, FString> MapName;

	EDifficulty gameDifficulty = EDifficulty::ED_Easy;

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

	virtual void Init() override;

	bool bBeforeWasMap = false;



};

