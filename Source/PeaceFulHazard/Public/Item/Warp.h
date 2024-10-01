// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HappyInteractableItem.h"
#include "Warp.generated.h"

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API AWarp : public AHappyInteractableItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warp Parameter")
	EWarpTarget WarpTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AdditiveStaticMesh1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AdditiveStaticMesh2;

	virtual void BeginPlay() override;

	virtual void AfterInteraction() override;

	UFUNCTION()
	void WarpDisable();

public:
	virtual void InteractWithPlayer(APeaceFulHazardCharacter* character) override;

	AWarp();

};
