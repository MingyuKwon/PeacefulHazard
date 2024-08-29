// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/PeaceFulHazardGameMode.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "System/PeaceFulPlayerStart.h"
#include "System/PeacFulGameInstance.h"
#include "System/MapNameStore.h"

APeaceFulHazardGameMode::APeaceFulHazardGameMode()
{

}



void APeaceFulHazardGameMode::SavePlayerParaBeforeWarp(FCharacterInventoty CharacterInventoty, FCharacterItemBox CharacterItemBox, int32 maxBullet, int32 currentBullet, float currentHealth, EItemType currentBulletType, bool Equipped)
{
    if (PeacFulGameInstance == nullptr) return;

    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    gameSave->SavedPlayerInventory = CharacterInventoty;
    gameSave->SavedPlayerCharacterItemBox = CharacterItemBox;
    gameSave->SavedPlayerMaxBullet = maxBullet;
    gameSave->SavedPlayerCurrentBullet = currentBullet;
    gameSave->SavedPlayerCurrentHealth = currentHealth;
    gameSave->SavedPlayercurrentBulletType = currentBulletType;
    gameSave->SavePlayerdEquipped = Equipped;
}

bool APeaceFulHazardGameMode::GetPlayerParaAfterWarp(FCharacterInventoty& CharacterInventoty, FCharacterItemBox& CharacterItemBox, int32& maxBullet, int32& currentBullet, float& currentHealth, EItemType& currentBulletType, bool& Equipped)
{
    if (PeacFulGameInstance == nullptr) return false;

    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    if (gameSave->bFirstGame) {

        gameSave->bFirstGame = false;
        return false;
    }

    CharacterInventoty = gameSave->SavedPlayerInventory;
    CharacterItemBox = gameSave->SavedPlayerCharacterItemBox;
    maxBullet = gameSave->SavedPlayerMaxBullet;
    currentBullet = gameSave->SavedPlayerCurrentBullet;
    currentHealth = gameSave->SavedPlayerCurrentHealth;
    currentBulletType = gameSave->SavedPlayercurrentBulletType;
    Equipped = gameSave->SavePlayerdEquipped;


    return true;
}


bool APeaceFulHazardGameMode::GetEnemyStats(FString name, float& enemyHealth, FVector& enemyLocation, FRotator& enemyRotation)
{
    if (PeacFulGameInstance == nullptr) return false;

    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    TMap<FString, FEnemySave> tempEnemyMap = gameSave->EnemySaveMap.FindOrAdd(currentMapType).enemySaves;

    if (!tempEnemyMap.Contains(name)) return false;
    
    enemyHealth = tempEnemyMap[name].enemyHealth;
    enemyLocation = tempEnemyMap[name].enemyLocation;
    enemyRotation = tempEnemyMap[name].enemyRotation;

    return true;

}

void APeaceFulHazardGameMode::SaveEnemyStats(FString name, float enemyHealth, FVector enemyLocation, FRotator enemyRotation)
{
    if (PeacFulGameInstance == nullptr) return;

    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    TMap<FString, FEnemySave> tempEnemyMap = gameSave->EnemySaveMap.FindOrAdd(currentMapType).enemySaves;

    enemyHealth = tempEnemyMap.FindOrAdd(name).enemyHealth;
    enemyLocation = tempEnemyMap.FindOrAdd(name).enemyLocation;
    enemyRotation = tempEnemyMap.FindOrAdd(name).enemyRotation;

}

bool APeaceFulHazardGameMode::CheckAleradyInteract(FString name)
{
    if (PeacFulGameInstance == nullptr) return true;
    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    TArray<FString> tempInteractMap = gameSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames;

    return tempInteractMap.Contains(name);
}

void APeaceFulHazardGameMode::SetAleradyInteract(FString name)
{
    if (PeacFulGameInstance == nullptr) return;
    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    TArray<FString> tempInteractMap = gameSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames;

    if (!tempInteractMap.Contains(name))
    {
        UE_LOG(LogTemp, Warning, TEXT("SetAleradyInteract %s"), *name);
        gameSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames.Add(name);
    }
}

void APeaceFulHazardGameMode::OpenMap(FString MapName)
{
    PeacFulGameInstance->beforeMapType = currentMapType;
    UGameplayStatics::OpenLevel(this, FName(*MapName));

}

AActor* APeaceFulHazardGameMode::ChoosePlayerStart_Implementation(AController* Player)
{

    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(this, APeaceFulPlayerStart::StaticClass(), PlayerStarts);

    PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(this));

    if (PeacFulGameInstance)
    {
        for (AActor* playerStart : PlayerStarts)
        {
            APeaceFulPlayerStart* temp = Cast<APeaceFulPlayerStart>(playerStart);

            if (temp->mapType == PeacFulGameInstance->beforeMapType)
            {
                return playerStart;
            }

        }
    }



    return Super::ChoosePlayerStart_Implementation(Player);

}

void APeaceFulHazardGameMode::BeginPlay()
{
    Super::BeginPlay();

    PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(this));

    AMapNameStore* mapStore = Cast<AMapNameStore>(UGameplayStatics::GetActorOfClass(this, AMapNameStore::StaticClass()));
    if (mapStore)
    {
        currentMapType = mapStore->mapType;
    }


    FTimerHandle startDelayHandle;
    GetWorld()->GetTimerManager().SetTimer(startDelayHandle, [this]()
        {
            MapStartEvent.Broadcast();
        }, 0.2f, false);

}
