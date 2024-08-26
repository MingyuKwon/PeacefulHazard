// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
class UBehaviorTreeComponent;
class UBlackboardComponent;
struct FAIStimulus;
class AEnemyBase;
class APeaceFulHazardCharacter;
class AEnemyRoutePivot;

UCLASS()
class PEACEFULHAZARD_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	float currentHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	float maxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	float damageAccumulate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	float stundamageAccumulateUnit = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	bool bStunHeadShot = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	bool bStunDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	bool bDeath = false;

	void EnemyTakeDamge(float Damage, bool bHead);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	APeaceFulHazardCharacter* Target;

	void TriggerRoute(TArray<AEnemyRoutePivot*> Pivots);
protected:

	TArray<AEnemyRoutePivot*> RoutePivots;

	UFUNCTION()
	void UpdateBlackBoard();

	UFUNCTION()
	void  HeadStunRelease();

	UFUNCTION()
	void  DamageStunRelease();


	FTimerHandle updateTimerHandle;
	FTimerHandle HeadStunTimerHandle;
	FTimerHandle DamgeStunTimerHandle;

	virtual void BeginPlay() override;

	AEnemyBase* controlEnemy;

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;
	UPROPERTY()
	UBlackboardComponent* BlackboardComp;


	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	class UCrowdFollowingComponent* CrowdComp;
};
