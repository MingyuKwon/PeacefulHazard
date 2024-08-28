// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "Controller/HappyPlayerController.h"
#include "PeacFulGameInstance.generated.h"

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

	void SaveParaBeforeWarp(FCharacterInventoty CharacterInventoty, FCharacterItemBox CharacterItemBox, int32 maxBullet, int32 currentBullet,float currentHealth, EItemType currentBulletType, bool Equipped);
	bool GetParaAfterWarp(FCharacterInventoty& CharacterInventoty, FCharacterItemBox& CharacterItemBox, int32& maxBullet, int32& currentBullet, float& currentHealth, EItemType& currentBulletType, bool& Equipped);


protected:
	virtual void Init() override;

	FCharacterInventoty SavedCharacterInventory;
	FCharacterItemBox SavedCharacterItemBox;
	int32 SavedMaxBullet;
	int32 SavedCurrentBullet;
	float SavedCurrentHealth;
	EItemType SavedcurrentBulletType;
	bool SavedEquipped;

	bool bBeforeWasMap = false;
};

