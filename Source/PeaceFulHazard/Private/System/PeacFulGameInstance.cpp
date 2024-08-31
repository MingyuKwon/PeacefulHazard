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

}