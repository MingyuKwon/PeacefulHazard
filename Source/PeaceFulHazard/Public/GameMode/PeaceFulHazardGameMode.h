// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "Item/HappyInteractableItem.h"
#include "PeaceFulHazardGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNowAimingEvent, bool, flag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOuterChangeInventoryEvent, int32, itemIndex, EItemType, itemType, int32, itemCount, bool, bReplace);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUseItemEvent, EItemType, itemType,  bool, bItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCloseAllUIEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractWithItemUIEvent, EItemType, itemtype , int32, count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractSituationEvent, EInteractSituationType, situationType);


UCLASS(minimalapi)
class APeaceFulHazardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APeaceFulHazardGameMode();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FNowAimingEvent NowAimingEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOuterChangeInventoryEvent OuterChangeInventoryEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FUseItemEvent UseItemEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FCloseAllUIEvent CloseAllUIEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FInteractWithItemUIEvent InteractWithItemUIEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FInteractSituationEvent InteractSituationEvent;

};



