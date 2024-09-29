// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuGameMode.h"
#include "System/PeacFulGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "System/SettingSave.h"

AMainMenuGameMode::AMainMenuGameMode()
{
    BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundMusicComponent"));
    BGMAudioComponent->bAutoActivate = false;
    BGMAudioComponent->bIsUISound = true;

    UIAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("UIAudioComponent"));
    UIAudioComponent->bAutoActivate = false;
    UIAudioComponent->bIsUISound = true;

}

void AMainMenuGameMode::ChangeLanguageCallBack()
{
    LanguageChangeEvent.Broadcast();

}

ELanguage AMainMenuGameMode::GetCurrentLanguage()
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

void AMainMenuGameMode::LoadDataFromSlot(FString slotName, bool bNewGame)
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
                PeacFulGameInstance->currentEnemyForce = LoadedGame->saveEnemyForce;
                PeacFulGameInstance->gameDifficulty = LoadedGame->SavegameDifficulty;
            }

            UGameplayStatics::OpenLevel(this, *TravelMap[LoadedGame->saveMapName]);

        }
    }

}

void AMainMenuGameMode::LoadDataFromContinue()
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

void AMainMenuGameMode::SaveSettingValue()
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

void AMainMenuGameMode::LoadSettingValue()
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

void AMainMenuGameMode::PlayUISound(USoundBase* Sound, float VolumeScale)
{
    if (PeacFulGameInstance)
    {
        PeacFulGameInstance->PlayAudioComponent(EGameSoundType::EGST_UI, UIAudioComponent, Sound, VolumeScale);
    }
}


void AMainMenuGameMode::SetBGMVolume(float value)
{
    if (BGMAudioComponent && PeacFulGameInstance)
    {
        float Volume = PeacFulGameInstance->BGMVolume * 2.f;

        BGMAudioComponent->SetVolumeMultiplier(Volume);
    }

}

void AMainMenuGameMode::SetGameBrightness()
{
    if (PeacFulGameInstance == nullptr) return;

    if (!CachedPostProcessVolume)
    {
        CachedPostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
    }

    if (CachedPostProcessVolume)
    {
        float BrightnessTemp = PeacFulGameInstance->Brightness;
        CachedPostProcessVolume->Settings.AutoExposureBias = BrightnessTemp;
    }
}

void AMainMenuGameMode::GetSettingValue(float& mouse, float& mouseAim)
{
    if (PeacFulGameInstance)
    {
        mouse = PeacFulGameInstance->MouseSensitivity;
        mouseAim = PeacFulGameInstance->MouseAimSensitivity;
    }

}

void AMainMenuGameMode::BeginPlay()
{
    PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (PeacFulGameInstance)
    {
        PeacFulGameInstance->LanguageChangeEvent.AddDynamic(this, &ThisClass::ChangeLanguageCallBack);
    }
}
