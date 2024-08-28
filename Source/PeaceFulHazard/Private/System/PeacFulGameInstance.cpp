// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PeacFulGameInstance.h"

bool UPeacFulGameInstance::checkIsTutorialAlready(ETutorialType tutorial)
{
    if (!TutorialCheckMap.Contains(tutorial)) return true;

    bool temp = TutorialCheckMap[tutorial];
    TutorialCheckMap[tutorial] = true;

    return temp;
}

void UPeacFulGameInstance::SaveParaBeforeWarp(FCharacterInventoty CharacterInventoty, FCharacterItemBox CharacterItemBox, int32 maxBullet, int32 currentBullet, float currentHealth, EItemType currentBulletType, bool Equipped)
{
    bBeforeWasMap = true;

    SavedCharacterInventory = CharacterInventoty;
    SavedCharacterItemBox = CharacterItemBox;
    SavedMaxBullet = maxBullet;
    SavedCurrentBullet = currentBullet;
    SavedCurrentHealth = currentHealth;
    SavedcurrentBulletType = currentBulletType;
    SavedEquipped = Equipped;

}

bool UPeacFulGameInstance::GetParaAfterWarp(FCharacterInventoty& CharacterInventoty, FCharacterItemBox& CharacterItemBox, int32& maxBullet, int32& currentBullet, float& currentHealth, EItemType& currentBulletType, bool& Equipped)
{
    if (!bBeforeWasMap) return false;

    CharacterInventoty = SavedCharacterInventory;
    CharacterItemBox = SavedCharacterItemBox;
    maxBullet = SavedMaxBullet;
    currentBullet = SavedCurrentBullet;
    currentHealth = SavedCurrentHealth;
    currentBulletType = SavedcurrentBulletType;
    bBeforeWasMap = false;
    Equipped = SavedEquipped;

    return true;
}

void UPeacFulGameInstance::Init()
{
    Super::Init();

    TutorialCheckMap.Add(ETutorialType::ETT_MoveTutorial, false);
    TutorialCheckMap.Add(ETutorialType::ETT_Fire, false);
    TutorialCheckMap.Add(ETutorialType::ETT_InteractWithItem, false);
    TutorialCheckMap.Add(ETutorialType::ETT_InteractWithSituation, false);
    TutorialCheckMap.Add(ETutorialType::ETT_InteractWithOneWayDoor, false);
    TutorialCheckMap.Add(ETutorialType::ETT_InteractItemBox, false);

}