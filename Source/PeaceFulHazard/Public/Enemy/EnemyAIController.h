// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
class UBehaviorTreeComponent;

UCLASS()
class PEACEFULHAZARD_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;

	class UCrowdFollowingComponent* CrowdComp;
};
