// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PeacFulGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UPeacFulGameInstance::UpdateToDo()
{
    todoIndex++;

    todoIndex = FMath::Clamp(todoIndex, 0, currentToDos.Num() - 1);

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

    tempSaveGame = Cast<UPeacFulSaveGame>(UGameplayStatics::CreateSaveGameObject(UPeacFulSaveGame::StaticClass()));

}


void UPeacFulGameInstance::resetTempSave()
{
    tempSaveGame = nullptr;
    todoIndex = 0;
    SelectedSaveSlot = TEXT("");
}

void UPeacFulGameInstance::Init()
{
    Super::Init();

    RefreshGame();

    todoIndex = 0;

    currentToDos.Add(EPlayerToDo::EPTD_GetOutTutorialRoom);
    currentToDos.Add(EPlayerToDo::EPTD_LookAroundMainHub);

    ToDoMap.Add(EPlayerToDo::EPTD_GetOutTutorialRoom, FString("find a way to get out of this building"));
    ToDoMap.Add(EPlayerToDo::EPTD_LookAroundMainHub, FString("Look around for anything that might provide information about the treasure."));

    MapName.Add(EWarpTarget::EWT_Tutorial, FString("Tutorial Room"));
    MapName.Add(EWarpTarget::EWT_MainHub, FString("Main Hub"));


    TutorialMap.Add(ETutorialType::ETT_MoveTutorial, FString("Move : ASDW \n\nRun : Left Shift"));
    TutorialMap.Add(ETutorialType::ETT_InteractWithItem, FString("As you approach an item, a white indicator appears. Get closer, and it turns blue. \n\nPress the left mouse button to pick up or interact."));
    TutorialMap.Add(ETutorialType::ETT_Fire, FString("Aim : RightClick \nFire : LeftClick \nReload : R \nChange Bullet : Mouse wheel"));
    TutorialMap.Add(ETutorialType::ETT_InteractWithSituation, FString("As you approach an interactable object, a white indicator appears. Get closer, and it turns blue. \n\nPress the left mouse button to interact."));
    TutorialMap.Add(ETutorialType::ETT_InteractWithOneWayDoor, FString("Some doors only open from the other side. \n\nIf it won't open, find another way around."));
    TutorialMap.Add(ETutorialType::ETT_InteractItemBox, FString("If there's an item box, you can store items from your inventory in it. \n\nYou can also retrieve items from the box."));

}