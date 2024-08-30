// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "Item/HappyInteractableItem.h"
#include "Controller/HappyPlayerController.h"
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMapStartEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMapEndEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMenuUIChangeEvent, bool, bVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveFinishedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWantSaveEvent);

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
	FMenuUIChangeEvent MenuUIChangeEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FSaveFinishedEvent SaveFinishedEvent;


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPlayerDeathEvent PlayerDeathEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FTutorialEvent TutorialEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FMapTravelEvent MapTravelEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FMapStartEvent MapStartEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FMapEndEvent MapEndEvent;


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FWantSaveEvent WantSaveEvent;

	
	void SaveDataToSlot(FString slotName);
	void LoadDataFromSlot(FString slotName);
	void DeleteDataFromSlot(FString slotName);

	void SetEnemyRefCount(bool bPlus);
	void SetEnemySaveRefCount(bool bPlus);

	EWarpTarget currentMapType = EWarpTarget::EWT_None;


	void SavePlayerParaBeforeWarp(FCharacterInventoty CharacterInventoty, FCharacterItemBox CharacterItemBox, int32 maxBullet, int32 currentBullet, float currentHealth, EItemType currentBulletType, bool Equipped);
	bool GetPlayerParaAfterWarp(FCharacterInventoty& CharacterInventoty, FCharacterItemBox& CharacterItemBox, int32& maxBullet, int32& currentBullet, float& currentHealth, EItemType& currentBulletType, bool& Equipped);

	bool CheckAleradyInteract(FString name);
	void SetAleradyInteract(FString name);

	bool GetEnemyStats(FString name, float& enemyHealth, FVector& enemyLocation, FRotator& enemyRotation);
	void SaveEnemyStats(FString name, float enemyHealth, FVector enemyLocation, FRotator enemyRotation);

	void OpenMap(FString MapName);
protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void BeginPlay() override;

	void SaveTempToSlot();

	int32 enemyRefCount = 0;

	int32 enemySaveRefCount = 0;

	FString ReceivedMapName = FString("");

	FString ReceivedSlotName = FString("");

	UPeacFulGameInstance* PeacFulGameInstance;

};



