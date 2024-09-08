// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HappyInteractableItem.h"
#include "HappyInteractableSituation.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API AHappyInteractableSituation : public AHappyInteractableItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* InteractSuccessSound;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AdditiveStaticMesh1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AdditiveStaticMesh2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OneWayAllow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OneWayNotAllow;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	EInteractSituationType situationType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Para", meta = (AllowPrivateAccess = "true"))
	FString NoteText;


	virtual void BeginPlay() override;

	virtual void AfterInteraction() override;

	UFUNCTION()
	void CheckBroadCastItemIsMe(EInteractSituationType _situationType);

	UFUNCTION()
	void ListenTirggerOn(EInteractSituationType _situationType);

	
public:
	virtual void InteractWithPlayer(APeaceFulHazardCharacter* character) override;

	AHappyInteractableSituation();

};
