// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "EnemyBase.generated.h"

class UBoxComponent;
class UAnimMontage;
class APeaceFulHazardGameMode;
class USoundBase;
class UNiagaraComponent;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDifficultyFloat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Easy = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Normal = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Hard = 30.f;

};

UCLASS()
class PEACEFULHAZARD_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HeadBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackRangeBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* SpawnNiagaraPoint;

	bool ShowChasingNiagara();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Para")
	float PatrolMoveToRange = 40;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Para")
	float AttackRange = 120;

	void StopHeadStunMontage();
	void StopDamageStunMontage();

	UFUNCTION(BlueprintCallable)
	void Attack(bool bBossRange = false);

	UFUNCTION(BlueprintCallable)
	void AttackImpact(int32 index);

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void PlayAttackSound(int32 index);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Para")
	float PatrolMoveSpeed = 120;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Para")
	float ChasingMoveSpeed = 200;

protected:
	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* BeforeBombEffect;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* AfterBombEffect;

	FVector BombLocationSave = FVector::ZeroVector;


	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* HeadHitSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* AttackSound;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Para")
	EPlayerToDo spawnProgress = EPlayerToDo::EPTD_GetOutTutorialRoom;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	FDifficultyFloat stundamageAccumulateUnit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	FDifficultyFloat stunHeadDamageAccumulateUnit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	FDifficultyFloat MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Pawn Para")
	FDifficultyFloat EnemyDamageAmount;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Para")
	ERouteNum routeNum = ERouteNum::ERN_RouteNum1;

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;
	
	UFUNCTION(BlueprintCallable)
	void MapEndCallBack();

	UFUNCTION()
	void MapStartCallBack();

	UFUNCTION(BlueprintCallable)
	void WantToSaveCallBack();


	FTimerHandle updateTimerHandle;
	UFUNCTION()
	void UpdateValue();

	void FindEnemyRoutes();

	float dissolvePercent = 1.f;

	virtual void BeginPlay() override;


	virtual void PossessedBy(AController* NewController) override;

	void MatchYawWithController(float DeltaTime);


	UPROPERTY()
	class AEnemyAIController* EnemyAIController;

	UPROPERTY(EditAnywhere, Category = "AI Parameter")
	class UBehaviorTree* BehaviorTree = nullptr;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);


	UPROPERTY(EditAnywhere, Category = "Anim Para")
	UAnimMontage* HitRightMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Para")
	UAnimMontage* HitLeftMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Para")
	UAnimMontage* HitBackMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Para")
	UAnimMontage* HitFwdMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Para")
	UAnimMontage* HeadStunMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Para")
	UAnimMontage* DamageStunMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Para")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Para")
	UAnimMontage* AttackMontage2;

	void PlayHitMontage(FVector ShotDirection);


	float StunPercent = 0.f;

	void SetMaterialParaLerp(bool bDissolve, float value);


public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Para")
	bool bBoss = false;

	float GetDamageUnitbyDifficulty();
	float GetHeadUnitbyDifficulty();
	float GetMaxHealthbyDifficulty();
	float GetEnemyDamageAmountbyDifficulty();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
