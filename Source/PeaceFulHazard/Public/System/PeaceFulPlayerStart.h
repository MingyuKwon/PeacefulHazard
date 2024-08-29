// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "PeaceFulPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API APeaceFulPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map Parameter", meta = (AllowPrivateAccess = "true"))
	EWarpTarget mapType;
	
};
