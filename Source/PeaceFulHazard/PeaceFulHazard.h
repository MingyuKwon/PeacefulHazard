// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    EIT_None,
    EIT_Bullet_Noraml,
    EIT_Bullet_Big,
    EIT_HealPack_Small,
    EIT_HealPack_Big,
    EIT_KeyNormal,
    EIT_KeyBig,
    EIT_Bag,
    EIT_GraveYardDoorKey,
    EIT_MainCatheralDoorKey,
    EIT_CrossOverFirstDoorKey,
    EIT_CrossOverSecondDoorKey,
    EIT_GraveyardLockKey,
    EIT_CathedralLockKey,

    EIT_Max
};

UENUM(BlueprintType)
enum class ETutorialType : uint8
{
    ETT_None,
    ETT_MoveTutorial,
    ETT_Fire,
    ETT_InteractWithItem,
    ETT_InteractWithSituation,
    ETT_InteractWithOneWayDoor,
    ETT_InteractItemBox,

    ETT_Max
};

UENUM(BlueprintType)
enum class EWarpTarget : uint8
{
    EWT_None,
    EWT_MainHub,
    EWT_Tutorial,
    EWT_MainCathedral,
    EWT_MainCathedralSecond,

    EWT_CrossOver,
    EWT_GraveYard,
    EWT_LeftGarden,
    EWT_RightGarden,

    EWT_Max
};

UENUM(BlueprintType)
enum class EInteractSituationType : uint8
{
    EIST_None,
    EIST_OneWayDoor,
    EIST_ItemBox,
    EIST_SaveBox,
    EIST_NormalDoor,
    EIST_BigDoor,
    EIST_GraveYardDoor,
    EIST_MainCatheralDoor,
    EIST_CrossOverFirstDoor,
    EIST_CrossOverSecondDoor,
    EIST_GraveyardLock,
    EIST_CathedralLock,

    EIST_Note,

    EIST_Max
};

UENUM(BlueprintType)
enum class EUIState : uint8
{
    EUIS_None,
    EUIS_ItemBox,
    EUIS_Inventory,
    EUIS_ItemGet,
    EUIS_Note,

    EUIS_Notice,

    EUIS_Menu,

    EUIS_Max
};

UENUM(BlueprintType)
enum class ERouteNum : uint8
{
    ERN_None,
    ERN_RouteNum1,
    ERN_RouteNum2,
    ERN_RouteNum3,
    ERN_RouteNum4,
    ERN_RouteNum5,
    ERN_RouteNum6,
    ERN_RouteNum7,
    ERN_RouteNum8,
    ERN_RouteNum9,
    ERN_RouteNum10,

    ERN_Max
};

UENUM(BlueprintType)
enum class EPlayerToDo : uint8
{
    EPTD_None,
    EPTD_GetOutTutorialRoom,
    EPTD_LookAroundMainHub,

    EPTD_Max
};
