// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "MapNameStore.generated.h"

UCLASS()
class PEACEFULHAZARD_API AMapNameStore : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map Parameter", meta = (AllowPrivateAccess = "true"))
	EWarpTarget mapType;

	AMapNameStore();
	
};
