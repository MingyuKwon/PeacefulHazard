// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/PeaceFulHazardGameMode.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "System/PeaceFulPlayerStart.h"
#include "System/PeacFulGameInstance.h"
#include "System/MapNameStore.h"
#include "System/PeacFulSaveGame.h"

APeaceFulHazardGameMode::APeaceFulHazardGameMode()
{

}

void APeaceFulHazardGameMode::OpenMap(FString MapName)
{
    ReceivedMapName = MapName;

    if (enemyRefCount <= 0)
    {
        if (ReceivedMapName.IsEmpty()) return;

        if (PeacFulGameInstance)
        {
            FString temp;
            if (PeacFulGameInstance->GetCurrentToDo(temp) == EPlayerToDo::EPTD_GetOutTutorialRoom)
            {
                PeacFulGameInstance->UpdateToDo();
            }
        }
        UGameplayStatics::OpenLevel(this, FName(*ReceivedMapName));
    }
    else
    {
        MapEndEvent.Broadcast();
    }
}

void APeaceFulHazardGameMode::SetEnemyRefCount(bool bPlus)
{
    if (bPlus)
    {
        enemyRefCount++;
    }
    else
    {
        enemyRefCount--;
    }

    if (enemyRefCount <= 0)
    {
        if (ReceivedMapName.IsEmpty()) return;

        if (PeacFulGameInstance)
        {
            FString temp;
            if (PeacFulGameInstance->GetCurrentToDo(temp) == EPlayerToDo::EPTD_GetOutTutorialRoom)
            {
                PeacFulGameInstance->UpdateToDo();
            }
        }
        UGameplayStatics::OpenLevel(this, FName(*ReceivedMapName));
    }
}

void APeaceFulHazardGameMode::SetEnemySaveRefCount(bool bPlus)
{
    if (bPlus)
    {
        enemySaveRefCount++;
    }
    else
    {
        enemySaveRefCount--;
    }

    if (enemySaveRefCount <= 0)
    {
        SaveTempToSlot();
    }
}




void APeaceFulHazardGameMode::SaveDataToSlot(FString slotName)
{
    enemySaveRefCount = enemyRefCount + 1;
    ReceivedSlotName = slotName;

    if (enemySaveRefCount <= 0 )
    {
        SaveTempToSlot();
    }
    else
    {
        WantSaveEvent.Broadcast();
    }
}

void APeaceFulHazardGameMode::SaveTempToSlot()
{
    if (ReceivedSlotName.IsEmpty()) return;

    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    if (gameSave)
    {
        gameSave->TutorialCheckMap = PeacFulGameInstance->TutorialCheckMap;
        gameSave->saveMapName = currentMapType;
        gameSave->saveTodoIndex = PeacFulGameInstance->todoIndex;

        UGameplayStatics::SaveGameToSlot(gameSave, ReceivedSlotName, 0);
    }

    enemySaveRefCount = 0;
    ReceivedSlotName = FString("");
    SaveFinishedEvent.Broadcast();
}

void APeaceFulHazardGameMode::LoadDataFromSlot(FString slotName, bool bNewGame)
{
    if (bNewGame)
    {
        PeacFulGameInstance->RefreshGame();
        
        UE_LOG(LogTemp, Warning, TEXT("RefreshGame"));

        UGameplayStatics::OpenLevel(this, *TravelMap[EWarpTarget::EWT_Tutorial]);

    }
    else
    {
        if (!slotName.IsEmpty())
        {
            UPeacFulSaveGame* LoadedGame = Cast<UPeacFulSaveGame>(UGameplayStatics::LoadGameFromSlot(slotName, 0));

            if (LoadedGame)
            {
                PeacFulGameInstance->tempSaveGame = LoadedGame;
                PeacFulGameInstance->TutorialCheckMap = LoadedGame->TutorialCheckMap;
                PeacFulGameInstance->todoIndex = LoadedGame->saveTodoIndex;
            }

            UGameplayStatics::OpenLevel(this, *TravelMap[LoadedGame->saveMapName]);

        }
    }

    


}

void APeaceFulHazardGameMode::DeleteDataFromSlot(FString slotName)
{
    if (!slotName.IsEmpty())
    {
        UGameplayStatics::DeleteGameInSlot(slotName, 0);
        SaveFinishedEvent.Broadcast();
    }

}

void APeaceFulHazardGameMode::SavePlayerPara(FCharacterInventoty CharacterInventoty, FCharacterItemBox CharacterItemBox, int32 maxBullet, int32 currentBullet, float currentHealth, EItemType currentBulletType, bool Equipped, FVector PlayerPosition, FRotator PlayerRotation)
{
    if (PeacFulGameInstance == nullptr) return;

    UE_LOG(LogTemp, Warning, TEXT("SavePlayerParaBeforeWarp"));

    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    gameSave->bFirstGame = false;
    gameSave->SavedPlayerInventory = CharacterInventoty;
    gameSave->SavedPlayerCharacterItemBox = CharacterItemBox;
    gameSave->SavedPlayerMaxBullet = maxBullet;
    gameSave->SavedPlayerCurrentBullet = currentBullet;
    gameSave->SavedPlayerCurrentHealth = currentHealth;
    gameSave->SavedPlayercurrentBulletType = currentBulletType;
    gameSave->SavePlayerdEquipped = Equipped;
    gameSave->SavedPlayerLocation = PlayerPosition;
    gameSave->SavedPlayerRotation = PlayerRotation;
}

bool APeaceFulHazardGameMode::GetPlayerPara(FCharacterInventoty& CharacterInventoty, FCharacterItemBox& CharacterItemBox, int32& maxBullet, int32& currentBullet, float& currentHealth, EItemType& currentBulletType, bool& Equipped, FVector& PlayerPosition, FRotator& PlayerRotation, EWarpTarget& saveMap)
{
    if (PeacFulGameInstance == nullptr) return false;

    UE_LOG(LogTemp, Warning, TEXT("GetPlayerPara"));

    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    if (gameSave->bFirstGame) {

        gameSave->bFirstGame = false;
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("GetPlayerPara 2"));


    CharacterInventoty = gameSave->SavedPlayerInventory;
    CharacterItemBox = gameSave->SavedPlayerCharacterItemBox;
    maxBullet = gameSave->SavedPlayerMaxBullet;
    currentBullet = gameSave->SavedPlayerCurrentBullet;
    currentHealth = gameSave->SavedPlayerCurrentHealth;
    currentBulletType = gameSave->SavedPlayercurrentBulletType;
    Equipped = gameSave->SavePlayerdEquipped;
    PlayerPosition = gameSave->SavedPlayerLocation;
    PlayerRotation = gameSave->SavedPlayerRotation;
    saveMap = gameSave->saveMapName;

    return true;
}

void APeaceFulHazardGameMode::SaveEnemyStats(FString name, float enemyHealth, FVector enemyLocation, FRotator enemyRotation)
{
    if (PeacFulGameInstance == nullptr) return;

    UE_LOG(LogTemp, Warning, TEXT("SaveEnemyStats"));


    UPeacFulSaveGame* gameSave = PeacFulGameInstance->tempSaveGame;

    TMap<FString, FEnemySave> tempEnemyMap = gameSave->EnemySaveMap.FindOrAdd(currentMapType).enemySaves;

    gameSave->EnemySaveMap.FindOrAdd(currentMapType).enemySaves.FindOrAdd(name).enemyHealth = enemyHealth;
    gameSave->EnemySaveMap.FindOrAdd(currentMapType).enemySaves.FindOrAdd(name).enemyLocation = enemyLocation;
    gameSave->EnemySaveMap.FindOrAdd(currentMapType).enemySaves.FindOrAdd(name).enemyRotation = enemyRotation;

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
            UPeacFulSaveGame* tempSave = PeacFulGameInstance->tempSaveGame;

            if (temp->mapType == tempSave->saveMapName)
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
        }, 0.1f, false);

}


