// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EMainMenuType : uint8
{
    EMT_None,
    EMT_Default,
    EMT_Load,
    EMT_Setting,
    EMT_NewGame,
    EMT_GameStart,

    EMT_Max
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
    EIT_None,
    EIT_Force,

    EIT_Bullet_Noraml,
    EIT_Bullet_Big,
    EIT_HealPack_Small,
    EIT_HealPack_Big,
    EIT_KeyNormal,
    EIT_KeyBig,
    EIT_Bag,
    EIT_GraveYardDoorKey,
    EIT_MainCatheralDoorKey,
    EIT_MainCatheralDoorKey2,

    EIT_CrossOverFirstDoorKey,
    EIT_CrossOverSecondDoorKey,
    EIT_GraveyardLockKey,
    EIT_CathedralLockKey,
    EIST_GardenLockKey,

    EIST_Treasure,

    EIT_Max
};

UENUM(BlueprintType)
enum class EGameSoundType : uint8
{
    EST_None,

    EGST_SFX,
    EGST_BGM,
    EGST_UI,
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

    ETT_UIItemBox,
    ETT_UIGetItem,
    ETT_UISituation,
    ETT_UIMenu_Save,
    ETT_UIShowInventory,

    ETT_WarpExplain,

    ETT_FinalBattleExplain,
    ETT_FinalBattleTimeLimit,

    ETT_Max
};

UENUM(BlueprintType)
enum class EWarpTarget : uint8
{
    EWT_None,
    EWT_MainHub,
    EWT_Tutorial,
    EWT_MainCathedral,

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
    EIST_MainCatheralDoor2,

    EIST_CrossOverFirstDoor,
    EIST_CrossOverSecondDoor,
    EIST_GraveyardLock,
    EIST_CathedralLock,
    EIST_GardenLock,

    EIST_Note,

    EIST_RedTrigger,
    EIST_RedTriggerDoor,

    EIST_GreenTrigger,
    EIST_GreenTriggerDoor,

    EIST_YellowTrigger,
    EIST_YellowTriggerDoor,

    EIST_BlueTrigger,
    EIST_BlueTriggerDoor,

    EIST_FinalWarp,

    EIST_Max
};

UENUM(BlueprintType)
enum class EUIState : uint8
{
    EUIS_None,

    EUIS_Tab,

    EUIS_Menu,

    EUIS_Notice,

    EUIS_Max
};

UENUM(BlueprintType)
enum class EMenuType : uint8
{
    EMT_None,

    EMT_Map,

    EMT_Save,

    EMT_Option,

    EMT_Max
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
    ERN_RouteNum11,
    ERN_RouteNum12,
    ERN_RouteNum13,
    ERN_RouteNum14,
    ERN_RouteNum15,
    ERN_RouteNum16,
    ERN_RouteNum17,

    ERN_Max
};

UENUM(BlueprintType)
enum class EPlayerToDo : uint8
{
    EPTD_None,
    EPTD_GetOutTutorialRoom,
    EPTD_LookAroundMainHub,
    EPTD_FindKeyToGravetard,
    EPTD_SearchGravetard,
    EPTD_SearchRightGarden,
    EPTD_SearchCathedral,
    EPTD_FindKeytoLeftGarden,
    EPTD_SearchLeftGarden,
    EPTD_SearchCathedralSecondFloor,
    EPTD_GetTreasure,
    EPTD_Survive,

    EPTD_Max
};

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
    ED_None,
    ED_Easy,
    ED_Normal,
    ED_Hard,

    ED_Max,

};

UENUM(BlueprintType)
enum class ELanguage : uint8
{
    EL_None,
    ED_English,
    ED_Korean,

    EL_Max,

};