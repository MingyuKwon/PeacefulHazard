// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "EnemyRoutePivot.generated.h"

UCLASS()
class PEACEFULHAZARD_API AEnemyRoutePivot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyRoutePivot();

	UPROPERTY(EditAnywhere, Category = "Pivot Para")
	ERouteNum HitRightMontage;

	UPROPERTY(EditAnywhere, Category = "Pivot Para")
	int32 index;

protected:

public:	

};
