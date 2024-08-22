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
/**
 * 
 */
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

	void Tab(const FInputActionValue& Value);

	void ChangeInventoryState();

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
	UInputAction* TabAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Parameter")
	TSubclassOf<AItemInformation> ItemInformationClass;

	AItemInformation* ItemInformation;

	class APlayerHUD* PlayerHUD;
	class APeaceFulHazardCharacter* ControlledCharacter;
	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	UFUNCTION(BlueprintCallable)
	void OuterUIChange(int32 itemIndex, EItemType itemType, int32 itemCount, bool bReplace);

	UFUNCTION(BlueprintCallable)
	void UseItem(EItemType itemType, bool bItem);

	

protected:

	FCharacterInventoty CharacterInventoty;

	bool nowPausing = false;

	int32 currentBullet = 6;
	int32 maxBullet = 6;

	int32 GetLeftBullet();
	int32 GetLockIndex();

	void UpdateAllUI();

	void UpdateInventoryUI();

	void UpdateDefaultUI();

	void InitializeInventory();


	UFUNCTION(BlueprintCallable)
	bool ChangeItemInventoryMap(EItemType itemType, int32 count);

	UFUNCTION(BlueprintCallable)
	bool ChangeItemInventoryArray(EItemType itemType, int32 count);

	UFUNCTION(BlueprintCallable)
	bool ChangeItemInventoryArrayOneSlot(int32 itemIndex, EItemType itemType, int32 itemCount, bool bReplace);


public:
	void SetBulletCount(bool bFire);

	UFUNCTION(BlueprintCallable)
	void ChangeItemInventory(EItemType itemType, int32 count);

};
