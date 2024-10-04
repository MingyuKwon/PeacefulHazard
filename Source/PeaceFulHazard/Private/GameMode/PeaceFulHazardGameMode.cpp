// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/PeaceFulHazardGameMode.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "System/PeaceFulPlayerStart.h"
#include "System/PeacFulGameInstance.h"
#include "System/MapNameStore.h"
#include "System/PeacFulSaveGame.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "System/SettingSave.h"

APeaceFulHazardGameMode::APeaceFulHazardGameMode()
{
    BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundMusicComponent"));
    BGMAudioComponent->bAutoActivate = false;  
    BGMAudioComponent->bIsUISound = true;

    UIAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("UIAudioComponent"));
    UIAudioComponent->bAutoActivate = false;
    UIAudioComponent->bIsUISound = true;
    
}


void APeaceFulHazardGameMode::OpenMap(FString MapName)
{
    ReceivedMapName = MapName;

    if (enemyRefCount <= 0)
    {
        if (ReceivedMapName.IsEmpty()) return;

        if (PeacFulGameInstance)
        {
            if (GetPlayerToDo() == EPlayerToDo::EPTD_GetOutTutorialRoom)
            {
                ToDoUpdate(EPlayerToDo::EPTD_LookAroundMainHub);
            }
        }
        UGameplayStatics::OpenLevel(this, FName(*ReceivedMapName));
    }
    else
    {
        MapEndEvent.Broadcast();
    }
}

void APeaceFulHazardGameMode::MoveToMainMenu()
{
    UGameplayStatics::OpenLevel(this, FName("MainMenu"));

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
            if (GetPlayerToDo() == EPlayerToDo::EPTD_GetOutTutorialRoom)
            {
                ToDoUpdate(EPlayerToDo::EPTD_LookAroundMainHub);
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




void APeaceFulHazardGameMode::FinalBattleTimeFunction()
{
    FinalBattleTimeSecond += 1;

    if (FinalBattleTimeSecond == 1)
    {
        DynamicSpawnStartEvent.Broadcast();

    }else if (FinalBattleTimeSecond == 5)
    {
        DynamicSpawnStartEvent.Broadcast();

    }
    else if (FinalBattleTimeSecond == 10)
    {
        DynamicSpawnStartEvent.Broadcast();

    }
    else if (FinalBattleTimeSecond == 180)
    {
        DynamicSpawnStartEvent.Broadcast();

    }
    else if (FinalBattleTimeSecond == 240)
    {
        DynamicSpawnStartEvent.Broadcast();

    }
}

void APeaceFulHazardGameMode::SaveSettingValue()
{
    if (PeacFulGameInstance == nullptr) return;

    USettingSave* settingSave = Cast<USettingSave>(UGameplayStatics::CreateSaveGameObject(USettingSave::StaticClass()));


    if (settingSave)
    {
        settingSave->MouseAimSensitivity = PeacFulGameInstance->MouseAimSensitivity;
        settingSave->MouseSensitivity = PeacFulGameInstance->MouseSensitivity;

        settingSave->BGMVolume = PeacFulGameInstance->BGMVolume;
        settingSave->SFXVolume = PeacFulGameInstance->SFXVolume;
        settingSave->UIVolume = PeacFulGameInstance->UIVolume;

        settingSave->Brightness = PeacFulGameInstance->Brightness;
        settingSave->Resolution = PeacFulGameInstance->Resolution;
        settingSave->Language = PeacFulGameInstance->Language;


        UGameplayStatics::SaveGameToSlot(settingSave, FString("Setting"), 0);
    }

}

void APeaceFulHazardGameMode::LoadSettingValue()
{
    if (UGameplayStatics::DoesSaveGameExist(FString("Setting"), 0))
    {
        USettingSave* LoadedSave = Cast<USettingSave>(UGameplayStatics::LoadGameFromSlot(FString("Setting"), 0));

        if (LoadedSave)
        {
            if (PeacFulGameInstance)
            {
                PeacFulGameInstance->MouseAimSensitivity = LoadedSave->MouseAimSensitivity;
                PeacFulGameInstance->MouseSensitivity = LoadedSave->MouseSensitivity;

                PeacFulGameInstance->BGMVolume = LoadedSave->BGMVolume;
                PeacFulGameInstance->SFXVolume = LoadedSave->SFXVolume;
                PeacFulGameInstance->UIVolume = LoadedSave->UIVolume;

            }
        }
    }
    else
    {
        if (PeacFulGameInstance)
        {
            PeacFulGameInstance->ResetSetting();
        }
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
        gameSave->saveEnemyForce = PeacFulGameInstance->currentEnemyForce;
        gameSave->saveTodoIndex = PeacFulGameInstance->todoIndex;
        gameSave->SaveTime = FDateTime::Now();
        gameSave->SavegameDifficulty = PeacFulGameInstance->gameDifficulty;

        UGameplayStatics::SaveGameToSlot(gameSave, ReceivedSlotName, 0);
    }

    enemySaveRefCount = 0;
    ReceivedSlotName = FString("");
    SaveFinishedEvent.Broadcast();
}

void APeaceFulHazardGameMode::LoadDataFromSlot(FString slotName, bool bNewGame)
{
    if (PeacFulGameInstance == nullptr) return;

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
                PeacFulGameInstance->currentEnemyForce = LoadedGame->saveEnemyForce;
                PeacFulGameInstance->gameDifficulty = LoadedGame->SavegameDifficulty;
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

    gameSave->saveMapName = currentMapType;

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

void APeaceFulHazardGameMode::ChangeLanguageCallBack()
{
    LanguageChangeEvent.Broadcast();
}

void APeaceFulHazardGameMode::BeginPlay()
{
    Super::BeginPlay();

    PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (PeacFulGameInstance)
    {
        PeacFulGameInstance->LanguageChangeEvent.AddDynamic(this, &ThisClass::ChangeLanguageCallBack);
    }

    AMapNameStore* mapStore = Cast<AMapNameStore>(UGameplayStatics::GetActorOfClass(this, AMapNameStore::StaticClass()));
    if (mapStore)
    {
        currentMapType = mapStore->mapType;
    }

    SetGameBrightness();

    FTimerHandle startDelayHandle;
    GetWorld()->GetTimerManager().SetTimer(startDelayHandle, [this]()
        {
            MapStartEvent.Broadcast();
            PlayBGM();
        }, 0.1f, false);


    if (GetPlayerToDo() == EPlayerToDo::EPTD_Survive)
    {
        DynamicSpawnMode = true;
    }

    CinematicPlayEvent.AddDynamic(this, &ThisClass::DynamimcSpawnStart);
    PlayerDeathEvent.AddDynamic(this, &ThisClass::PlayerDeath);
}

void APeaceFulHazardGameMode::PlayerDeath()
{
    if (!CachedPostProcessVolume)
    {
        CachedPostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
    }

    GetWorld()->GetTimerManager().SetTimer(BrightnessTimerHandle, this, &APeaceFulHazardGameMode::DecreaseBrightness, 0.1f, true);

    if (PeacFulGameInstance == nullptr) return;
    PeacFulGameInstance->PlayAudioComponent(EGameSoundType::EGST_BGM, BGMAudioComponent, GameOverMusic, 1.f);

}

void APeaceFulHazardGameMode::DecreaseBrightness()
{
    static int32 Counter = 0;  // 반복 횟수를 세기 위한 변수

    if (Counter < 15)
    {
        if (CachedPostProcessVolume)
        {
            CachedPostProcessVolume->Settings.AutoExposureBias -= 0.3f;
        }
        Counter++;
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(BrightnessTimerHandle);
        Counter = 0;  // 카운터 초기화
    }
}


void APeaceFulHazardGameMode::SetGameBrightness()
{
    if (PeacFulGameInstance == nullptr) return;

    if (!CachedPostProcessVolume)
    {
        CachedPostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
    }
    if (CachedPostProcessVolume)
    {
        float BrightnessTemp = PeacFulGameInstance->Brightness;

        switch (currentMapType)
        {
        case EWarpTarget::EWT_Tutorial:
            BrightnessTemp *= 1.1f;
            break;
        case EWarpTarget::EWT_MainHub:
            BrightnessTemp *= 0.8f;
            break;
        case EWarpTarget::EWT_CrossOver:
            BrightnessTemp *= 1.1f;
            break;
        case EWarpTarget::EWT_GraveYard:
            BrightnessTemp *= 0.7f;
            break;
        case EWarpTarget::EWT_LeftGarden:
            BrightnessTemp *= 0.8f;
            break;
        case EWarpTarget::EWT_MainCathedral:
            BrightnessTemp *= 1.1f;
            break;
        case EWarpTarget::EWT_RightGarden:
            BrightnessTemp *= 0.8f;
            break;
        }

        CachedPostProcessVolume->Settings.AutoExposureBias = BrightnessTemp;


        float TemperatureTemp = 6500.0f;  // 기본 색온도 (6500K는 중립적인 색온도)
        FLinearColor TintTemp = FLinearColor::White;  // 기본 색조

        FString temp;

        float TransitionSpeed = 0.5f;  // 색상 변화 속도 조절 (0.0f ~ 1.0f)
        FLinearColor StartTint = FLinearColor(0.8f, 0.9f, 1.0f);  // 초기 푸른빛
        FLinearColor EndTint = FLinearColor(1.0f, 0.75f, 0.5f);   // 석양빛 (노란빛)
        float StartTemperature = 5000.0f;  // 초기 푸른빛 온도
        float EndTemperature = 8500.0f;    // 석양빛 온도


        switch (PeacFulGameInstance->GetCurrentToDo(temp))
        {
        case EPlayerToDo::EPTD_GetOutTutorialRoom:
        case EPlayerToDo::EPTD_LookAroundMainHub:
        case EPlayerToDo::EPTD_FindKeyToGravetard:
        case EPlayerToDo::EPTD_SearchGravetard:
            TemperatureTemp = 5000.0f;  // 차가운 색 온도 (푸른빛)
            TintTemp = FLinearColor(0.5f, 0.7f, 1.0f);  // 푸른색조, 더 차갑게 보이도록 변경
            UE_LOG(LogTemp, Display, TEXT("Bright 1"));
            break;

        case EPlayerToDo::EPTD_SearchRightGarden:
            TemperatureTemp = 6000.0f;  // 약간 따뜻해진 색 온도
            TintTemp = FLinearColor(0.8f, 0.6f, 0.7f);  // 보라색 느낌의 중간 단계
            UE_LOG(LogTemp, Display, TEXT("Bright 2"));

            break;

        case EPlayerToDo::EPTD_SearchCathedral:
        case EPlayerToDo::EPTD_FindKeytoLeftGarden:
            TemperatureTemp = 7000.0f;  // 따뜻한 색 온도
            TintTemp = FLinearColor(1.0f, 0.6f, 0.6f);  // 붉은빛과 약간의 따뜻한 색조
            UE_LOG(LogTemp, Display, TEXT("Bright 3"));

            break;

        case EPlayerToDo::EPTD_SearchLeftGarden:
            TemperatureTemp = 7500.0f;  // 더 따뜻한 색 온도
            TintTemp = FLinearColor(1.0f, 0.4f, 0.4f);  // 붉은 계열로 변화
            UE_LOG(LogTemp, Display, TEXT("Bright 4"));

            break;

        case EPlayerToDo::EPTD_SearchCathedralSecondFloor:
            TemperatureTemp = 8000.0f;  // 매우 따뜻한 색 온도
            TintTemp = FLinearColor(1.0f, 0.3f, 0.3f);  // 강렬한 붉은색
            UE_LOG(LogTemp, Display, TEXT("Bright 5"));

            break;

        case EPlayerToDo::EPTD_GetTreasure:
        case EPlayerToDo::EPTD_Survive:
            TemperatureTemp = 8500.0f;  // 극도로 따뜻한 색 온도
            TintTemp = FLinearColor(1.0f, 0.2f, 0.2f);  // 매우 진한 붉은색
            UE_LOG(LogTemp, Display, TEXT("Bright 6"));

            break;
        }


        CachedPostProcessVolume->Settings.WhiteTemp = TemperatureTemp;
        CachedPostProcessVolume->Settings.ColorSaturation = TintTemp; 
    }

}

void APeaceFulHazardGameMode::LoadDataFromContinue()
{
    TArray<FString> slotName;
    slotName.Add(FString("SaveButton1"));
    slotName.Add(FString("SaveButton2"));
    slotName.Add(FString("SaveButton3"));
    slotName.Add(FString("SaveButton4"));
    slotName.Add(FString("SaveButton5"));
    slotName.Add(FString("SaveButton6"));
    slotName.Add(FString("SaveButton7"));
    slotName.Add(FString("SaveButton8"));

    FString LatestSlotName = FString();
    FDateTime LatestSaveTime = FDateTime::MinValue();

    for (FString name : slotName)
    {
        if (UGameplayStatics::DoesSaveGameExist(name, 0))
        {
            UPeacFulSaveGame* LoadedGame = Cast<UPeacFulSaveGame>(UGameplayStatics::LoadGameFromSlot(name, 0));
            if (LoadedGame)
            {
                FDateTime LoadedSaveTime = LoadedGame->SaveTime;

                if (LoadedSaveTime > LatestSaveTime)
                {
                    LatestSaveTime = LoadedSaveTime;
                    LatestSlotName = name;
                }
            }
        }
    }

    if (!LatestSlotName.IsEmpty())
    {
        LoadDataFromSlot(LatestSlotName, false);
    }
}


void APeaceFulHazardGameMode::PlayBGM()
{
    if (currentMapType == EWarpTarget::EWT_None) return;
    if (PeacFulGameInstance == nullptr) return;


    if (BackgroundMusics.Contains(currentMapType))
    {
        USoundBase* playSound = BackgroundMusics[currentMapType];

        if (currentMapType == EWarpTarget::EWT_MainHub && GetPlayerToDo() == EPlayerToDo::EPTD_Survive)
        {
            playSound = FinalBattleBGM;
        }
        
        PeacFulGameInstance->PlayAudioComponent(EGameSoundType::EGST_BGM ,BGMAudioComponent, playSound, 1.f);
    }
    
}



void APeaceFulHazardGameMode::PlaySoundInGameplay(USoundBase* Sound, FVector Location, float VolumeScale)
{
    if (PeacFulGameInstance)
    {
        PeacFulGameInstance->PlaySoundOnceInGamePlay(Sound, Location, VolumeScale);
    }
}

void APeaceFulHazardGameMode::PlayUISound(USoundBase* Sound, float VolumeScale)
{
    if (PeacFulGameInstance)
    {
        PeacFulGameInstance->PlayAudioComponent(EGameSoundType::EGST_UI, UIAudioComponent, Sound, VolumeScale);
    }
}

void APeaceFulHazardGameMode::SetBGMVolume(float value)
{
    if (BGMAudioComponent && PeacFulGameInstance)
    {
        float Volume = PeacFulGameInstance->BGMVolume * 2.f;

        BGMAudioComponent->SetVolumeMultiplier(Volume);
    }

}

EPlayerToDo APeaceFulHazardGameMode::GetPlayerToDo()
{
    if (PeacFulGameInstance)
    {
        FString temp;
        return PeacFulGameInstance->GetCurrentToDo(temp);
    }

    return EPlayerToDo::EPTD_None;
}

EDifficulty APeaceFulHazardGameMode::GetDifficulty()
{
    if (PeacFulGameInstance == nullptr) return EDifficulty::ED_None;

    return PeacFulGameInstance->gameDifficulty;
}

void APeaceFulHazardGameMode::ToDoUpdate(EPlayerToDo targetTodo)
{
    if (PeacFulGameInstance)
    {
        if (GetPlayerToDo() >= targetTodo) return;

        PeacFulGameInstance->UpdateToDo(targetTodo);
        SetGameBrightness();
    }
}

void APeaceFulHazardGameMode::DynamimcSpawnStart(bool play)
{
    if (!play)
    {
        if (DynamicSpawnMode)
        {
            GetWorld()->GetTimerManager().SetTimer(finalBattleTimerHandle, this, &ThisClass::FinalBattleTimeFunction, 1.f, true);
        }
    }
}

ELanguage APeaceFulHazardGameMode::GetCurrentLanguage()
{
    if (PeacFulGameInstance)
    {
        if (PeacFulGameInstance->Language == "English")
        {
            return ELanguage::ED_English;
        }
        else if (PeacFulGameInstance->Language == FString(TEXT("한국어")))
        {
            return ELanguage::ED_Korean;

        }
    }

    return ELanguage::EL_None;
}

void APeaceFulHazardGameMode::GetSettingValue(float& mouse, float& mouseAim)
{
    if (PeacFulGameInstance)
    {
        mouse = PeacFulGameInstance->MouseSensitivity;
        mouseAim = PeacFulGameInstance->MouseAimSensitivity;


    }
}


