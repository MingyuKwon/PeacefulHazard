// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "EnemyBase.generated.h"

class UBoxComponent;
class UAnimMontage;
class APeaceFulHazardGameMode;
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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Para")
	float PatrolMoveToRange = 40;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Para")
	float AttackRange = 120;

	void StopHeadStunMontage();
	void StopDamageStunMontage();

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackImpact(int32 index);

	UFUNCTION(BlueprintCallable)
	void AttackEnd();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Para")
	float PatrolMoveSpeed = 120;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Para")
	float ChasingMoveSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Para")
	float EnemyDamageAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Para")
	ERouteNum routeNum = ERouteNum::ERN_None;

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;
	
	UFUNCTION(BlueprintCallable)
	void MapEndCallBack();

	UFUNCTION()
	void MapStartCallBack();


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


	void PlayHitMontage(FVector ShotDirection);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
