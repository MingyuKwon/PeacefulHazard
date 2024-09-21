// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PeacFulGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "System/SettingSave.h"
#include "GameFramework/GameUserSettings.h"

void UPeacFulGameInstance::UpdateToDo(EPlayerToDo targetTodo)
{
    int32 todoIndexInArray = currentToDos.Find(targetTodo);
    if (todoIndexInArray != INDEX_NONE)
    {
        todoIndex = todoIndexInArray;
    }

    todoIndex = todoIndexInArray;

    todoIndex = FMath::Clamp(todoIndex, 0, currentToDos.Num() - 1);

    switch (targetTodo)
    {
    case EPlayerToDo::EPTD_SearchRightGarden :
        currentEnemyForce += 5;
        break;

    case EPlayerToDo::EPTD_SearchCathedral:
        currentEnemyForce += 5;
        break;

    case EPlayerToDo::EPTD_SearchLeftGarden:
        currentEnemyForce += 10;
        break;

    case EPlayerToDo::EPTD_SearchCathedralSecondFloor:
        currentEnemyForce += 10;
        break;

    case EPlayerToDo::EPTD_Survive:
        currentEnemyForce += 20;
        break;

      
    }


}

EPlayerToDo UPeacFulGameInstance::GetCurrentToDo(FString& TodoString)
{
    todoIndex = FMath::Clamp(todoIndex, 0, currentToDos.Num() - 1);

    TodoString = ToDoMap[currentToDos[todoIndex]];

    return currentToDos[todoIndex];
}

bool UPeacFulGameInstance::checkIsTutorialAlready(ETutorialType tutorial)
{
    if (!TutorialCheckMap.Contains(tutorial)) return true;

    bool temp = TutorialCheckMap[tutorial];
    TutorialCheckMap[tutorial] = true;

    return temp;
}

void UPeacFulGameInstance::RefreshGame()
{
    TutorialCheckMap.Empty();

    TutorialCheckMap.Add(ETutorialType::ETT_MoveTutorial, false);
    TutorialCheckMap.Add(ETutorialType::ETT_Fire, false);
    TutorialCheckMap.Add(ETutorialType::ETT_InteractWithItem, false);
    TutorialCheckMap.Add(ETutorialType::ETT_InteractWithSituation, false);
    TutorialCheckMap.Add(ETutorialType::ETT_InteractWithOneWayDoor, false);
    TutorialCheckMap.Add(ETutorialType::ETT_InteractItemBox, false);
    TutorialCheckMap.Add(ETutorialType::ETT_UIGetItem, false);
    TutorialCheckMap.Add(ETutorialType::ETT_UISituation, false);
    TutorialCheckMap.Add(ETutorialType::ETT_UIItemBox, false);
    TutorialCheckMap.Add(ETutorialType::ETT_UIMenu_Save, false);
    TutorialCheckMap.Add(ETutorialType::ETT_UIShowInventory, false);

    tempSaveGame = Cast<UPeacFulSaveGame>(UGameplayStatics::CreateSaveGameObject(UPeacFulSaveGame::StaticClass()));

}


void UPeacFulGameInstance::resetTempSave()
{
    tempSaveGame = nullptr;
    todoIndex = 0;
    SelectedSaveSlot = TEXT("");
}

void UPeacFulGameInstance::PlaySoundOnceInGamePlay(USoundBase* Sound, FVector Location, float VolumeScale)
{
    float Volume = 2 * SFXVolume * VolumeScale;

    if (Sound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, Sound, Location, Volume);
    }
}

void UPeacFulGameInstance::PlayAudioComponent(EGameSoundType soundType, UAudioComponent* AudioComponent, USoundBase* Sound, float VolumeScale)
{
    float Volume = 1.f;

    switch (soundType)
    {
    case EGameSoundType::EGST_BGM:
        Volume = BGMVolume * 2;
        break;

    case EGameSoundType::EGST_UI:
        Volume = UIVolume * 2;
        break;

    default:
        break;
    }

    Volume *= VolumeScale;

    if (Sound)
    {
        AudioComponent->SetSound(Sound);
        AudioComponent->SetVolumeMultiplier(Volume);
        AudioComponent->Play();
    }
    
}



void UPeacFulGameInstance::ResetSetting()
{
    UE_LOG(LogTemp, Display, TEXT("ResetSetting"));

    MouseSensitivity = 0.4f;
    MouseAimSensitivity = 0.4f;

    SFXVolume = 0.5f;
    BGMVolume = 0.5f;
    UIVolume = 0.5f;

    Brightness = 14.f;

    Resolution = FString("1920 x 1080");
}

void UPeacFulGameInstance::LoadSettingValue()
{
    if (UGameplayStatics::DoesSaveGameExist(FString("Setting"), 0))
    {
        USettingSave* LoadedSave = Cast<USettingSave>(UGameplayStatics::LoadGameFromSlot(FString("Setting"), 0));

        if (LoadedSave)
        {
            MouseAimSensitivity = LoadedSave->MouseAimSensitivity;
            MouseSensitivity = LoadedSave->MouseSensitivity;

            BGMVolume = LoadedSave->BGMVolume;
            SFXVolume = LoadedSave->SFXVolume;
            UIVolume = LoadedSave->UIVolume;

            Brightness = LoadedSave->Brightness;

            Resolution = LoadedSave->Resolution;
        }
    }
    else
    {
        ResetSetting();
    }
}

void UPeacFulGameInstance::SetResolution(FString SelectedItem)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();

    Resolution = SelectedItem;

    if (UserSettings)
    {
        if (Resolution == "1920 x 1080")
        {
            UserSettings->SetScreenResolution(FIntPoint(1920, 1080));
        }
        else if (Resolution == "2560 x 1440")
        {
            UserSettings->SetScreenResolution(FIntPoint(2560, 1440));
        }
        else if (Resolution == "1600 x 900")
        {
            UserSettings->SetScreenResolution(FIntPoint(1600, 900));
        }
        else if (Resolution == "1280 x 720")
        {
            UserSettings->SetScreenResolution(FIntPoint(1280, 720));
        }

        UserSettings->SetFullscreenMode(EWindowMode::Windowed);

        UserSettings->ApplySettings(false);
    }

}

void UPeacFulGameInstance::Init()
{
    Super::Init();

    RefreshGame();

    todoIndex = 0;

    currentToDos.Add(EPlayerToDo::EPTD_GetOutTutorialRoom);
    currentToDos.Add(EPlayerToDo::EPTD_LookAroundMainHub);
    currentToDos.Add(EPlayerToDo::EPTD_FindKeyToGravetard);
    currentToDos.Add(EPlayerToDo::EPTD_SearchGravetard);
    currentToDos.Add(EPlayerToDo::EPTD_SearchRightGarden);
    currentToDos.Add(EPlayerToDo::EPTD_SearchCathedral);
    currentToDos.Add(EPlayerToDo::EPTD_FindKeytoLeftGarden);
    currentToDos.Add(EPlayerToDo::EPTD_SearchLeftGarden);
    currentToDos.Add(EPlayerToDo::EPTD_SearchCathedralSecondFloor);
    currentToDos.Add(EPlayerToDo::EPTD_GetTreasure);
    currentToDos.Add(EPlayerToDo::EPTD_Survive);


    ToDoMap.Add(EPlayerToDo::EPTD_GetOutTutorialRoom, FString("Find a way to get out of this building"));
    ToDoMap.Add(EPlayerToDo::EPTD_LookAroundMainHub, FString("Look around for anything that might provide information about the \"treasure\"."));
    ToDoMap.Add(EPlayerToDo::EPTD_FindKeyToGravetard, FString("Explore a way to access the locked \"graveyard\""));
    ToDoMap.Add(EPlayerToDo::EPTD_SearchGravetard, FString("Investigate the \"graveyard\" to see if we can find any useful clues."));
    ToDoMap.Add(EPlayerToDo::EPTD_SearchRightGarden, FString("Investigate the \"right garden\" to see if we can find any useful clues."));
    ToDoMap.Add(EPlayerToDo::EPTD_SearchCathedral, FString("Head to the \"cathedral\" to get the treasure."));
    ToDoMap.Add(EPlayerToDo::EPTD_FindKeytoLeftGarden, FString("The clue to access the second floor of the \"cathedral\" is in the \"left garden\". Get the key to the door that leads to the \"left garden\"."));
    ToDoMap.Add(EPlayerToDo::EPTD_SearchLeftGarden, FString("Investigate the \"left garden\" to see if we can find any useful clues."));
    ToDoMap.Add(EPlayerToDo::EPTD_SearchCathedralSecondFloor, FString("Investigate the \"cathedral\" second floor"));
    ToDoMap.Add(EPlayerToDo::EPTD_GetTreasure, FString("Get the Treasure!"));
    ToDoMap.Add(EPlayerToDo::EPTD_Survive, FString("Survive within the time limit"));



    MapName.Add(EWarpTarget::EWT_Tutorial, FString("Tutorial Room"));
    MapName.Add(EWarpTarget::EWT_MainHub, FString("Main Hub"));


    TutorialMap.Add(ETutorialType::ETT_MoveTutorial, FString("Move : ASDW \n\nRun : Left Shift \n\nInteract : Left Mouse Button \n\nBack : Right Mouse Button"));
    TutorialMap.Add(ETutorialType::ETT_InteractWithItem, FString("As you approach an item, a white indicator appears. Get closer, and it turns blue. \n\nPress the left mouse button to pick up or interact."));
    TutorialMap.Add(ETutorialType::ETT_Fire, FString("Aim : RightClick \nFire : LeftClick \nReload : R \nChange Bullet : Mouse wheel"));
    TutorialMap.Add(ETutorialType::ETT_InteractWithSituation, FString("As you approach an interactable object, a white indicator appears. Get closer, and it turns blue. \n\nPress the left mouse button to interact."));
    TutorialMap.Add(ETutorialType::ETT_InteractWithOneWayDoor, FString("Some doors only open from the other side. \n\nIf it won't open, find another way around."));
    TutorialMap.Add(ETutorialType::ETT_InteractItemBox, FString("If there's an item box, you can store items from your inventory in it. \n\nYou can also retrieve items from the box."));

    TutorialMap.Add(ETutorialType::ETT_UIGetItem, FString("When you get item, you can take it by pressing left mouse button.\n(You only can take item if there is empty space) \n\nIf you don't want to take it, press right mouse button."));
    TutorialMap.Add(ETutorialType::ETT_UISituation, FString("You can iteract by using special item. \n\ninteractable items will be showen brighter and clickable."));
    TutorialMap.Add(ETutorialType::ETT_UIItemBox, FString("You can store items from your inventory in the box or take items from the box into your inventory. \n\nMake sure there's an empty space in either the inventory or the box where you want to place the item."));
    TutorialMap.Add(ETutorialType::ETT_UIMenu_Save, FString("Press the desired slot button to save or load data. \nIf you want to delete a saved file, click the X button above. \n\nTo start a new game, press New Game at the top."));
    TutorialMap.Add(ETutorialType::ETT_UIShowInventory, FString("In the inventory, you can use, combine, discard items. \n\nSome items may have different options for use or whether they can be discarded."));


    
    LoadSettingValue();

    SetResolution(Resolution);

}