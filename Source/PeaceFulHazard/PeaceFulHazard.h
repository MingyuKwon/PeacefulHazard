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
    EIT_Max
};

UENUM(BlueprintType)
enum class EInteractSituationType : uint8
{
    EIST_None,
    EIST_Bullet_Noraml,
    EIST_Max
};

UENUM(BlueprintType)
enum class EUIState : uint8
{
    EUIS_None,
    EUIS_Inventory,
    EUIS_ItemGet,
    EUIS_Max
};
