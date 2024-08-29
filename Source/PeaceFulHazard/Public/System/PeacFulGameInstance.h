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

	bool checkIsTutorialAlready(ETutorialType tutorial);


	EWarpTarget beforeMapType = EWarpTarget::EWT_None;


	UPROPERTY();
	UPeacFulSaveGame* tempSaveGame;
	FString SelectedSaveSlot = FString("");

	void resetTempSave();

protected:
	virtual void Init() override;

	bool bBeforeWasMap = false;
};

