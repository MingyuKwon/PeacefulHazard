// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "Item/ItemInformation.h"
#include "HappyPlayerController.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class APlayerHUD;
struct FInputActionValue;
class AHappyPlayerController;
class AWeapon;
class UAnimMontage;
class APeaceFulHazardGameMode;
class AHappyInteractableItem;
class UPeacFulGameInstance;

USTRUCT(BlueprintType)
struct FCharacterInventoty
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TArray<EItemType> inventoryItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TArray<int32> inventoryItemCounts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TArray<bool> ItemLockArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EItemType, int32> ItemCountMap;
};

USTRUCT(BlueprintType)
struct FCharacterItemBox
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TArray<EItemType> itemBoxItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TArray<int32> itemBoxItemCounts;
};


UCLASS()
class PEACEFULHAZARD_API AHappyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Aim input */
	void RIghtClickStart(const FInputActionValue& Value);

	/** Called for Aim input */
	void RightClickEnd(const FInputActionValue& Value);

	/** Called for Fire input */
	void Action(const FInputActionValue& Value);

	/** Called for Fire input */
	void ShiftStart(const FInputActionValue& Value);

	/** Called for Fire input */
	void ShiftEnd(const FInputActionValue& Value);

	/** Called for Fire input */
	void EquipTrigger(const FInputActionValue& Value);

	/** Called for Fire input */
	void Reload(const FInputActionValue& Value);

	void ChangeBullet(const FInputActionValue& Value);

	void Tab(const FInputActionValue& Value);

	void Menu(const FInputActionValue& Value);


	void PauseGame(bool flag);

	int32 GetReloadBulletCount();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShiftAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BulletChangeAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TabAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MenuAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Parameter")
	TSubclassOf<AItemInformation> ItemInformationClass;

	AItemInformation* ItemInformation;


	UFUNCTION(BlueprintCallable)
	void WantToSaveCallBack();


	EUIState currentUIState = EUIState::EUIS_None;

	void ChangeUiState(EUIState uiState, bool bLock);

	bool bNoticeLock = false;
	bool bMenuLock = false;
	bool bTabLock = false;

	class APlayerHUD* PlayerHUD;
	class APeaceFulHazardCharacter* ControlledCharacter;
	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	UPeacFulGameInstance* PeacFulGameInstance;

	UFUNCTION(BlueprintCallable)
	void CloseAllUI();

	UFUNCTION(BlueprintCallable)
	void ShowMainMenuUI(bool bVisible);

	UFUNCTION(BlueprintCallable)
	void MapStartCallBack();


	UFUNCTION(BlueprintCallable)
	void OuterUIChange(int32 itemIndex, EItemType itemType, int32 itemCount, bool bReplace);

	UFUNCTION(BlueprintCallable)
	void UseItem(EItemType itemType, bool bItem);

	UFUNCTION(BlueprintCallable)
	void InteractWithItemUI(EItemType itemtype, int32 count);

	UFUNCTION(BlueprintCallable)
	void SituationInteract(EInteractSituationType situationType);

	UFUNCTION(BlueprintCallable)
	void ShowLoadingUI(bool bVisible);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "map Parameter")
	TMap<EWarpTarget, FString> TravelMap;


	UFUNCTION(BlueprintCallable)
	void TutorialShow(ETutorialType tutorialType);


	// Map Loading Needed Para
	FCharacterInventoty CharacterInventoty;
	FCharacterItemBox CharacterItemBox;

	int32 maxBullet = 15;
	int32 currentBullet = 10;

	float currentHealth = 50.f;

	void SetHealth(float changeAmount);

	EItemType currentBulletType = EItemType::EIT_Bullet_Noraml;

	// Map Loading Needed Para

	bool nowPausing = false;
	void SetGamePause(bool flag);


	int32 normalBulletMax = 15;
	int32 BigBulletMax = 8;

	int32 GetLeftBullet();
	int32 GetAnotherBullet();

	int32 GetLockIndex();

	void UpdateAllUI();

	void UpdateInventoryUI();

	void UpdateItemBoxUI();


	void UpdateDefaultUI();

	void InitializeInventory();


	UFUNCTION(BlueprintCallable)
	bool ChangeItemInventoryMap(EItemType itemType, int32 count);

	UFUNCTION(BlueprintCallable)
	bool ChangeItemInventoryArray(EItemType itemType, int32 count);

	UFUNCTION(BlueprintCallable)
	bool ChangeItemInventoryArrayOneSlot(int32 itemIndex, EItemType itemType, int32 itemCount, bool bReplace);

	UFUNCTION(BlueprintCallable)
	void ChangeItemInventory(EItemType itemType, int32 count);

	UFUNCTION(BlueprintCallable)
	void InventorySizeUpgrade(int32 count);

	bool CanGetItem(EItemType itemType, int32 count);
	bool IsInventoryFull();


	bool bCinematicShow = false;

	UFUNCTION(BlueprintCallable)
	void CinematicShow(bool bShow);

	UFUNCTION(BlueprintCallable)
	void ChangeItemBoxInContrller(int32 index, EItemType itemType, int32 count);

	void ReArrangeBox();

	UFUNCTION(BlueprintCallable)
	void ChangeItemBox(bool bInventroyToBox, int32 index, EItemType itemtype, int32 count);

	UFUNCTION(BlueprintCallable)
	void ShowNoticeUI(bool bVisible, FString& noticeText);


	UFUNCTION(BlueprintCallable)
	void WarpTravel(EWarpTarget warptarget);



	FTimerHandle UpdateValueHandle;

	UFUNCTION(BlueprintCallable)
	void UpdateValue();

	UFUNCTION()
	void SetOnceCheckDisplay(bool bVisible, const FText EnglishText = FText(), const FText KoreanText = FText());

	
public:
	float aimFocusLerpValue = 1.f;

	UFUNCTION(BlueprintCallable)
	void ShowInformationUI(bool bVisible, FString& noticeText);


	void SetBulletCount(bool bFire);
	void SetBulletChangeCount();

	int32 GetPlayerForce();

	void GameClear();


	UFUNCTION(BlueprintCallable)
	void GetItem(EItemType itemType, int32 count);

	UFUNCTION(BlueprintCallable)
	void TriggerSituation(EInteractSituationType situtaionType);

	UFUNCTION(BlueprintCallable)
	void TriggerItemBox();

	UFUNCTION(BlueprintCallable)
	void TriggerMenu_Save(bool bSave);

	UFUNCTION()
	void TakeDamge(float damage);

	UFUNCTION(BlueprintCallable)
	EItemType GetcurrentBulletType() { return currentBulletType; }

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() { return currentHealth  / 50.f; }

};
