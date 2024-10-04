// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HappyInteractableItem.h"
#include "HappyPichUpItem.generated.h"

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API AHappyPichUpItem : public AHappyInteractableItem
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Para", meta = (AllowPrivateAccess = "true"))
	bool bFinalBattle = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Para", meta = (AllowPrivateAccess = "true"))
	int32 WaveCount = 1;

	int32 CurrentWaveCount = 0;

	void SetActorVisibility(bool flag);

	UFUNCTION()
	void CheckToShow();



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	int32 EasyItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	int32 NormalItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	int32 HardItemCount;

	virtual void BeginPlay() override;

	virtual void AfterInteraction() override;


	UFUNCTION()
	void CheckBroadCastItemIsMe(EItemType _itemtype, int32 _count);

	void EnalbleCustomDepth(bool bEnable);


public:
	virtual void InteractWithPlayer(APeaceFulHazardCharacter* character) override;

	AHappyPichUpItem();

};
