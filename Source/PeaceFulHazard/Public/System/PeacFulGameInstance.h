// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "Controller/HappyPlayerController.h"
#include "System/PeacFulSaveGame.h"
#include "PeacFulGameInstance.generated.h"

class UPeacFulSaveGame;
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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TArray<EPlayerToDo> currentToDos;
	int32 todoIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EPlayerToDo, FString> ToDoMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EWarpTarget, FString> MapName;

	int32 currentEnemyForce = 0;

	void UpdateToDo();

	EPlayerToDo GetCurrentToDo(FString& TodoString);

	bool checkIsTutorialAlready(ETutorialType tutorial);

	void RefreshGame();

	UPROPERTY();
	UPeacFulSaveGame* tempSaveGame;
	FString SelectedSaveSlot = FString("");

	void resetTempSave();

protected:
	virtual void Init() override;

	bool bBeforeWasMap = false;
};

