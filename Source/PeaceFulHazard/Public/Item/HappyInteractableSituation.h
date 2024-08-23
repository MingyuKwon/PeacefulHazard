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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AdditiveStaticMesh1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AdditiveStaticMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	EInteractSituationType situationType;

	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckBroadCastItemIsMe(EInteractSituationType _situationType);

	
public:
	virtual void InteractWithPlayer(APeaceFulHazardCharacter* character) override;

	AHappyInteractableSituation();

};
