// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HappyInteractableItem.h"
#include "HappyInteractableSituation.generated.h"

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API AHappyInteractableSituation : public AHappyInteractableItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	EInteractSituationType situationType;

public:
	virtual void InteractWithPlayer(APeaceFulHazardCharacter* character) override;

};
