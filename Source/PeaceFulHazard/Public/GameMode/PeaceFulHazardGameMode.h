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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FItemBoxInteractEvent, bool, bInventroyToBox, int32, index, EItemType, itemtype, int32, count );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNoticeUIShowEvent, bool, bVisible, FString&, noticeText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTutorialEvent, ETutorialType, tutorialType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMapTravelEvent, EWarpTarget, warptarget);

class UPeacFulGameInstance;


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

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FItemBoxInteractEvent ItemBoxInteractEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FNoticeUIShowEvent NoticeUIShowEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPlayerDeathEvent PlayerDeathEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FTutorialEvent TutorialEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FMapTravelEvent MapTravelEvent;

	EWarpTarget currentMapType = EWarpTarget::EWT_None;

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void BeginPlay() override;


	UPeacFulGameInstance* PeacFulGameInstance;

};



