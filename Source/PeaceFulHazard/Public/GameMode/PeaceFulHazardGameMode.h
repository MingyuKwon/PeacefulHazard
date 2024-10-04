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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInformationUIShowEvent, bool, bVisible, FString&, noticeText);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTriggerDoorEvent, EInteractSituationType, situationType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTutorialEvent, ETutorialType, tutorialType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMapTravelEvent, EWarpTarget, warptarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMapStartEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMapEndEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMenuUIChangeEvent, bool, bVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveFinishedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWantSaveEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FModeLanguageChangeEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicSpawnStartEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowLoadingEvent, bool, bVisible);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCinematicPlayEvent, bool, bPlay);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCheckOneMoreGameEvent, bool, bVisible, const FText, EnglishText, const FText, KoreanText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCheckOneMoreSuccessGameEvent);



class UPeacFulGameInstance;
class USoundBase;
class UAudioComponent;
class APostProcessVolume;

UCLASS(minimalapi)
class APeaceFulHazardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APeaceFulHazardGameMode();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FModeLanguageChangeEvent LanguageChangeEvent;

	UFUNCTION()
	void ChangeLanguageCallBack();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FShowLoadingEvent ShowLoadingEvent;

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
	FInformationUIShowEvent InformationUIShowEvent;


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FTriggerDoorEvent TriggerDoorEvent;


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

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FDynamicSpawnStartEvent DynamicSpawnStartEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FCinematicPlayEvent CinematicPlayEvent;


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FCheckOneMoreGameEvent CheckOneMoreGameEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FCheckOneMoreSuccessGameEvent CheckOneMoreSuccessGameEvent;

	UPROPERTY(BlueprintReadWrite)
	int32 FinalBattleTimeSecond = 0;

	
	void SaveSettingValue();
	void LoadSettingValue();

	ELanguage GetCurrentLanguage();
	void GetSettingValue(float& mouse, float& mouseAim);

	void SaveDataToSlot(FString slotName);
	void LoadDataFromSlot(FString slotName, bool bNewGame);
	void DeleteDataFromSlot(FString slotName);

	void SetEnemyRefCount(bool bPlus);
	void SetEnemySaveRefCount(bool bPlus);

	EWarpTarget currentMapType = EWarpTarget::EWT_None;


	void SavePlayerPara(FCharacterInventoty CharacterInventoty, FCharacterItemBox CharacterItemBox, int32 maxBullet, int32 currentBullet, float currentHealth, EItemType currentBulletType, bool Equipped, FVector PlayerPosition, FRotator PlayerRotation);
	bool GetPlayerPara(FCharacterInventoty& CharacterInventoty, FCharacterItemBox& CharacterItemBox, int32& maxBullet, int32& currentBullet, float& currentHealth, EItemType& currentBulletType, bool& Equipped, FVector& PlayerPosition, FRotator& PlayerRotation, EWarpTarget& saveMap);

	bool CheckAleradyInteract(FString name);
	void SetAleradyInteract(FString name);

	bool GetEnemyStats(FString name, float& enemyHealth, FVector& enemyLocation, FRotator& enemyRotation);
	void SaveEnemyStats(FString name, float enemyHealth, FVector enemyLocation, FRotator enemyRotation);

	void OpenMap(FString MapName);

	void MoveToMainMenu();

	void PlaySoundInGameplay(USoundBase* Sound, FVector Location, float VolumeScale);
	void PlayUISound(USoundBase* Sound, float VolumeScale);
	void SetBGMVolume(float value);


	void SetGameBrightness();

	void LoadDataFromContinue();

	UFUNCTION(BlueprintCallable)
	EPlayerToDo GetPlayerToDo();

	EDifficulty GetDifficulty();

	void ToDoUpdate(EPlayerToDo targetTodo);


protected:

	FTimerHandle finalBattleTimerHandle;
	UFUNCTION()
	void FinalBattleTimeFunction();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	bool DynamicSpawnMode = false;

	UFUNCTION(BlueprintCallable)
	void DynamimcSpawnStart(bool play);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TMap<EWarpTarget, FString> TravelMap;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TMap<EWarpTarget, USoundBase*> BackgroundMusics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* FinalBattleBGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* GameClearMusic;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* GameOverMusic;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* BGMAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* UIAudioComponent;


	void PlayBGM();

	UFUNCTION()
	void PlayerDeath();

	UFUNCTION()
	void DecreaseBrightness();

	FTimerHandle BrightnessTimerHandle;


	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void BeginPlay() override;

	void SaveTempToSlot();

	int32 enemyRefCount = 0;

	int32 enemySaveRefCount = 0;

	FString ReceivedMapName = FString("");

	FString ReceivedSlotName = FString("");

	UPeacFulGameInstance* PeacFulGameInstance;

	APostProcessVolume* CachedPostProcessVolume;

};



