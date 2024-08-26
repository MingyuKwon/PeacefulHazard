// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UBoxComponent;
class UAnimMontage;

UCLASS()
class PEACEFULHAZARD_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HeadBox;

	void StopHeadStunMontage();
	void StopDamageStunMontage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

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
	UAnimMontage* DeathMontage;



	void PlayHitMontage(FVector ShotDirection);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
