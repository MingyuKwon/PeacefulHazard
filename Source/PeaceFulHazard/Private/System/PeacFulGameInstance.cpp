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
        currentEnemyForce += 10;
        break;

    case EPlayerToDo::EPTD_SearchCathedral:
        currentEnemyForce += 15;
        break;

    case EPlayerToDo::EPTD_SearchLeftGarden:
        currentEnemyForce += 20;
        break;

    case EPlayerToDo::EPTD_SearchCathedralSecondFloor:
        currentEnemyForce += 20;
        break;

    case EPlayerToDo::EPTD_Survive:
        currentEnemyForce += 10;
        break;

      
    }


}

EPlayerToDo UPeacFulGameInstance::GetCurrentToDo(FString& TodoString)
{
    todoIndex = FMath::Clamp(todoIndex, 0, currentToDos.Num() - 1);

    TodoString = GetToDoMapByLanguage()[currentToDos[todoIndex]];

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

    todoIndex = 0;


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
    TutorialCheckMap.Add(ETutorialType::ETT_WarpExplain, false);
    TutorialCheckMap.Add(ETutorialType::ETT_FinalBattleExplain, false);
    TutorialCheckMap.Add(ETutorialType::ETT_FinalBattleTimeLimit, false);

    
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
            Language = LoadedSave->Language;
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

void UPeacFulGameInstance::SetLangauage(FString SelectedItem)
{
    Language = SelectedItem;

    if (Language == "English")
    {

    }
    else if (Language == FString(TEXT("한국어")))
    {

    }

    LanguageChangeEvent.Broadcast();
}

TMap<EPlayerToDo, FString> UPeacFulGameInstance::GetToDoMapByLanguage()
{
    if (Language == "English")
    {
        return ToDoMapEnglish;
    }
    else if (Language == FString(TEXT("한국어")))
    {
        return ToDoMapKorean;
    }

    return ToDoMapEnglish;
}

TMap<EWarpTarget, FString> UPeacFulGameInstance::GetMapNameByLanguage()
{
    if (Language == "English")
    {
        return MapNameEnglish;
    }
    else if (Language == FString(TEXT("한국어")))
    {
        return MapNameKorean;
    }

    return MapNameEnglish;
}

TMap<ETutorialType, FString> UPeacFulGameInstance::GetTutorialByLanguage()
{
    if (Language == "English")
    {
        return TutorialMapEnglish;
    }
    else if (Language == FString(TEXT("한국어")))
    {
        return TutorialMapKorean;
    }

    return TutorialMapEnglish;
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


    ToDoMapEnglish.Add(EPlayerToDo::EPTD_GetOutTutorialRoom, FString("Find a way to get out of this building"));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_LookAroundMainHub, FString("Look around for anything that might provide information about the \"treasure\"."));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_FindKeyToGravetard, FString("Explore a way to access the locked \"graveyard\""));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_SearchGravetard, FString("Investigate the \"graveyard\" to see if we can find any useful clues."));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_SearchRightGarden, FString("Investigate the \"right garden\" to see if we can find any useful clues."));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_SearchCathedral, FString("Head to the \"cathedral\" to get the treasure."));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_FindKeytoLeftGarden, FString("The clue to access the second floor of the \"cathedral\" is in the \"left garden\". Get the key to the door that leads to the \"left garden\"."));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_SearchLeftGarden, FString("Investigate the \"left garden\" to see if we can find any useful clues."));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_SearchCathedralSecondFloor, FString("Investigate the \"cathedral\" second floor"));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_GetTreasure, FString("Get the Treasure!"));
    ToDoMapEnglish.Add(EPlayerToDo::EPTD_Survive, FString("Let's escape this area and complete the mission!"));

    ToDoMapKorean.Add(EPlayerToDo::EPTD_GetOutTutorialRoom, FString(TEXT("건물에서 나갈 방법을 찾자")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_LookAroundMainHub, FString(TEXT("보물에 대한 정보를 제공할 수 있는 무언가를 찾아 중앙광장을 둘러보자.")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_FindKeyToGravetard, FString(TEXT("잠긴 \"무덤\"으로 접근할 방법을 찾아보자.")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_SearchGravetard, FString(TEXT("\"무덤\"을 조사하여 유용한 단서를 찾을 수 있는지 확인하자.")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_SearchRightGarden, FString(TEXT("\"오른쪽 정원\"을 조사하여 유용한 단서를 찾을 수 있는지 확인하자.")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_SearchCathedral, FString(TEXT("\"성당\"으로 가서 보물을 찾자.")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_FindKeytoLeftGarden, FString(TEXT("\"성당\" 2층으로 가기 위한 단서는 \"왼쪽 정원\"에 있다. \"왼쪽 정원\"으로 가는 문 열쇠를 얻자.")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_SearchLeftGarden, FString(TEXT("\"왼쪽 정원\"을 조사하여 유용한 단서를 찾을 수 있는지 확인하자.")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_SearchCathedralSecondFloor, FString(TEXT("\"성당\" 2층을 조사하자.")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_GetTreasure, FString(TEXT("보물을 획득하자!")));
    ToDoMapKorean.Add(EPlayerToDo::EPTD_Survive, FString(TEXT("이 지역에서 탈출하고, 임무를 완수하자")));


    

    MapNameEnglish.Add(EWarpTarget::EWT_Tutorial, FString("Tutorial Room"));
    MapNameEnglish.Add(EWarpTarget::EWT_MainHub, FString("Main Hub"));
    MapNameEnglish.Add(EWarpTarget::EWT_MainCathedral, FString("Cathedral"));
    MapNameEnglish.Add(EWarpTarget::EWT_CrossOver, FString("CrossOver"));
    MapNameEnglish.Add(EWarpTarget::EWT_GraveYard, FString("Graveyard"));
    MapNameEnglish.Add(EWarpTarget::EWT_LeftGarden, FString("Left Garden"));
    MapNameEnglish.Add(EWarpTarget::EWT_RightGarden, FString("Right Garden"));

    MapNameKorean.Add(EWarpTarget::EWT_Tutorial, FString(TEXT("튜토리얼 방")));
    MapNameKorean.Add(EWarpTarget::EWT_MainHub, FString(TEXT("중앙 광장")));
    MapNameKorean.Add(EWarpTarget::EWT_MainCathedral, FString(TEXT("성당")));
    MapNameKorean.Add(EWarpTarget::EWT_CrossOver, FString(TEXT("교차로")));
    MapNameKorean.Add(EWarpTarget::EWT_GraveYard, FString(TEXT("무덤")));
    MapNameKorean.Add(EWarpTarget::EWT_LeftGarden, FString(TEXT("공원 왼쪽")));
    MapNameKorean.Add(EWarpTarget::EWT_RightGarden, FString(TEXT("공원 오른쪽")));


    TutorialMapEnglish.Add(ETutorialType::ETT_MoveTutorial, FString("Move : ASDW \nRun : Left Shift \n\nInteract : Left Mouse Button \nBack : Right Mouse Button\n\nE : Weapon Equip"));
    TutorialMapEnglish.Add(ETutorialType::ETT_InteractWithItem, FString("As you approach an item, a white indicator appears. Get closer, and it turns blue. \n\nPress the left mouse button to pick up or interact."));
    TutorialMapEnglish.Add(ETutorialType::ETT_Fire, FString("Aim : RightClick \nFire : LeftClick \nReload : R \nChange Bullet : Mouse wheel"));
    TutorialMapEnglish.Add(ETutorialType::ETT_InteractWithSituation, FString("As you approach an interactable object, a white indicator appears. Get closer, and it turns blue. \n\nPress the left mouse button to interact."));
    TutorialMapEnglish.Add(ETutorialType::ETT_InteractWithOneWayDoor, FString("Some doors only open from the other side. \n\nIf it won't open, find another way around."));
    TutorialMapEnglish.Add(ETutorialType::ETT_InteractItemBox, FString("If there's an item box, you can store items from your inventory in it. \n\nYou can also retrieve items from the box."));

    TutorialMapEnglish.Add(ETutorialType::ETT_UIGetItem, FString("When you get item, you can take it by pressing left mouse button.\n(You only can take item if there is empty space) \n\nIf you don't want to take it, press right mouse button."));
    TutorialMapEnglish.Add(ETutorialType::ETT_UISituation, FString("You can iteract by using special item. \n\ninteractable items will be showen brighter and clickable."));
    TutorialMapEnglish.Add(ETutorialType::ETT_UIItemBox, FString("You can store items from your inventory in the box or take items from the box into your inventory. \n\nMake sure there's an empty space in either the inventory or the box where you want to place the item."));
    TutorialMapEnglish.Add(ETutorialType::ETT_UIMenu_Save, FString("Press the desired slot button to save or load data. \nIf you want to delete a saved file, click the X button above. \n\nTo start a new game, press New Game at the top."));
    TutorialMapEnglish.Add(ETutorialType::ETT_UIShowInventory, FString("In the inventory, you can use, combine, discard items. \n\nSome items may have different options for use or whether they can be discarded."));

    TutorialMapEnglish.Add(ETutorialType::ETT_WarpExplain, FString("The doors with a green glowing outline are warp doors. \n\nBy interacting with them, you can enter or exit different buildings or areas, so make sure to use them actively."));
    TutorialMapEnglish.Add(ETutorialType::ETT_FinalBattleExplain, FString("Now that you’ve obtained the treasure, all that’s left is to return. \nHead to the central plaza and wait for rescue.\n\nHowever, the enemies won’t let you leave easily, so it’s best to prepare as much as possible before heading out of the cathedral."));
    TutorialMapEnglish.Add(ETutorialType::ETT_FinalBattleTimeLimit, FString("It will take 5 minutes for the rescue to arrive. \n\nYou must survive for those 5 minutes using every possible means!"));

    
    TutorialMapKorean.Add(ETutorialType::ETT_MoveTutorial, FString(TEXT("이동 : ASDW \n달리기 : 왼쪽 Shift \n\n상호작용 : 왼쪽 마우스 버튼 \n뒤로 가기 : 오른쪽 마우스 버튼\n\nE : 무기 장착")));
    TutorialMapKorean.Add(ETutorialType::ETT_InteractWithItem, FString(TEXT("아이템에 가까이 다가가면 흰색 표시가 나타납니다. 더 가까이 가면 파란색으로 변합니다. \n\n왼쪽 마우스 버튼을 눌러 아이템을 줍거나 상호작용하세요.")));
    TutorialMapKorean.Add(ETutorialType::ETT_Fire, FString(TEXT("조준 : 오른쪽 마우스 클릭 \n발사 : 왼쪽 마우스 클릭 \n재장전 : R \n탄환 변경 : 마우스 휠")));
    TutorialMapKorean.Add(ETutorialType::ETT_InteractWithSituation, FString(TEXT("상호작용 가능한 객체에 가까이 다가가면 흰색 표시가 나타납니다. 더 가까이 가면 파란색으로 변합니다. \n\n왼쪽 마우스 버튼을 눌러 상호작용하세요.")));
    TutorialMapKorean.Add(ETutorialType::ETT_InteractWithOneWayDoor, FString(TEXT("일부 문은 반대편에서만 열립니다. \n\n열리지 않으면 다른 길을 찾아보세요.")));
    TutorialMapKorean.Add(ETutorialType::ETT_InteractItemBox, FString(TEXT("아이템 박스가 있으면 인벤토리에서 아이템을 보관할 수 있습니다. \n\n박스에서 아이템을 꺼내올 수도 있습니다.")));

    TutorialMapKorean.Add(ETutorialType::ETT_UIGetItem, FString(TEXT("아이템을 얻으면 왼쪽 마우스 버튼을 눌러 가져올 수 있습니다.\n(빈 공간이 있어야만 아이템을 가져올 수 있습니다) \n\n아이템을 원하지 않으면 오른쪽 마우스 버튼을 누르세요.")));
    TutorialMapKorean.Add(ETutorialType::ETT_UISituation, FString(TEXT("특수 아이템을 사용하여 상호작용할 수 있습니다. \n\n상호작용 가능한 아이템은 더 밝게 표시되고 클릭할 수 있습니다.")));
    TutorialMapKorean.Add(ETutorialType::ETT_UIItemBox, FString(TEXT("인벤토리에서 아이템을 박스에 보관하거나 박스에서 아이템을 꺼내올 수 있습니다. \n\n아이템을 보관할 곳에 빈 공간이 있는지 확인하세요.")));
    TutorialMapKorean.Add(ETutorialType::ETT_UIMenu_Save, FString(TEXT("저장하거나 데이터를 불러오려면 원하는 슬롯 버튼을 누르세요. \n저장된 파일을 삭제하려면 위쪽의 X 버튼을 클릭하세요. \n\n새 게임을 시작하려면 상단의 새 게임 버튼을 누르세요.")));
    TutorialMapKorean.Add(ETutorialType::ETT_UIShowInventory, FString(TEXT("인벤토리에서 아이템을 사용, 결합, 버릴 수 있습니다. \n\n일부 아이템은 사용 방법이나 버릴 수 있는지 여부에 따라 다른 옵션이 있을 수 있습니다.")));

    TutorialMapKorean.Add(ETutorialType::ETT_WarpExplain, FString(TEXT("테두리가 녹색 빛이 나는 문들을 워프 문 입니다. \n\n워프 문과 상호작용 하면 다른 건물이나 지역으로 들어가거나 나올 수 있으므로 적극 활용 해 봅시다")));
    TutorialMapKorean.Add(ETutorialType::ETT_FinalBattleExplain, FString(TEXT("이제 보물을 획득했으니, 복귀 하는 일만 남았습니다. \n중앙 광장으로 가서 구조를 기다리세요. \n\n다만, 적들이 순순히 보내 줄 리가 없으므로 최대한 준비를 한 후에, 성당 밖으로 나가는 것이 좋습니다")));
    TutorialMapKorean.Add(ETutorialType::ETT_FinalBattleTimeLimit, FString(TEXT("구조까지 걸리는 시간을 5분 입니다. \n\n가능한 모든 수단을 동원해 5분 동안 살아남아야 합니다!")));

    

    
    LoadSettingValue();

    SetResolution(Resolution);

}