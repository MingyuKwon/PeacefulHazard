// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "PeacFulGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UPeacFulGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<ETutorialType, bool> TutorialCheckMap;

	bool checkIsTutorialAlready(ETutorialType tutorial);

protected:
	virtual void Init() override;

};

