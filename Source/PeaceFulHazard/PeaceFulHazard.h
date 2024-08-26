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

    EIT_Max
};

UENUM(BlueprintType)
enum class EInteractSituationType : uint8
{
    EIST_None,
    EIST_OneWayDoor,
    EIST_ItemBox,
    EIST_NormalDoor,
    EIST_Max
};

UENUM(BlueprintType)
enum class EUIState : uint8
{
    EUIS_None,
    EUIS_ItemBox,
    EUIS_Inventory,
    EUIS_ItemGet,
    EUIS_Notice,

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
