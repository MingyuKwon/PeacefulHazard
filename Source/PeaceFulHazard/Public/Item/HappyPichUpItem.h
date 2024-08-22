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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckBroadCastItemIsMe(EItemType _itemtype, int32 _count);


public:
	virtual void InteractWithPlayer(APeaceFulHazardCharacter* character) override;

};
