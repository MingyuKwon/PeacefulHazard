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
class APeaceFulHazardGameMode;

UCLASS()
class PEACEFULHAZARD_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	float currentHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	float maxHealth = 100.f;

	float damageAccumulate = 0.f;
	float headDamageAccumulate = 0.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	bool bStunHeadShot = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	bool bStunDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	bool bDeath = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	bool bNowAttacking = false;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	bool bFollowingLastPositon = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	int32 routeIndex = 0;

	void EnemyTakeDamge(float Damage, bool bHead);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	APeaceFulHazardCharacter* Target;

	void TriggerRoute(TArray<AEnemyRoutePivot*> Pivots);

	UFUNCTION(BlueprintCallable)
	void TriggerResetPivotIndex(bool BFollowingLastPosition);

protected:
	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	UFUNCTION()
	void GameClear();

	bool bGameClear = false;


	bool bSurviveMode = false;

	UFUNCTION()
	void PlayerDeathCallback();

	void Attack(bool bBossRange = false);

	UFUNCTION()
	bool CheckMovetoDestination();

	TArray<AEnemyRoutePivot*> RoutePivots;

	UFUNCTION()
	void UpdateBlackBoard();

	UFUNCTION()
	void  HeadStunRelease();

	UFUNCTION()
	void  DamageStunRelease();

	FTimerHandle nonAttackTimerHandle;
	bool nonAttackLock = true;

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
