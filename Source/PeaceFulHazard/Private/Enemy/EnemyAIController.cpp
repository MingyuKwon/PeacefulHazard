// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Character/PeaceFulHazardCharacter.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))

{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComponent"));
	check(Blackboard);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorTreeComponent"));
	check(BehaviorTreeComponent);

	CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());

	if (CrowdComp)
	{
		CrowdComp->SetCrowdObstacleAvoidance(false);
		CrowdComp->SetCrowdSeparation(false);
		CrowdComp->SetCrowdOptimizeTopology(false);
		CrowdComp->SetCrowdAnticipateTurns(true);
		CrowdComp->SetCrowdCollisionQueryRange(3000.f);

		CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);

	}


	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated);

}

void AEnemyAIController::EnemyTakeDamge(float Damage, bool bHead)
{
	currentHealth -= Damage;
	damageAccumulate += Damage;

	if (stundamageAccumulateUnit <= damageAccumulate)
	{
		damageAccumulate = 0;
		bStunDamage = true;
		GetWorld()->GetTimerManager().SetTimer(DamgeStunTimerHandle, [this]()
			{
				bStunDamage = false;
			}, 2.f, false);
	}

	if (bHead)
	{
		bStunHeadShot = true;
		GetWorld()->GetTimerManager().SetTimer(HeadStunTimerHandle, [this]()
			{
				bStunHeadShot = false;
			}, 0.5f, false);
	}

	if (currentHealth <= 0)
	{
		bDeath = true;
	}
}

void AEnemyAIController::UpdateBlackBoard()
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("bDeath"), bDeath);
		BlackboardComp->SetValueAsBool(TEXT("bStunDamage"), bStunDamage);
		BlackboardComp->SetValueAsBool(TEXT("bStunHeadShot"), bStunHeadShot);

		UE_LOG(LogTemp, Display, TEXT("UpdateBlackBoard"));
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	BlackboardComp = GetBlackboardComponent();
	GetWorld()->GetTimerManager().SetTimer(updateTimerHandle, this, &ThisClass::UpdateBlackBoard , 0.1f, true);
		
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor->IsA(APeaceFulHazardCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player detected: %s"), *Actor->GetName());
		}
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor->IsA(APeaceFulHazardCharacter::StaticClass())) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sense In: %s"), *Actor->GetName());

		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsObject(TEXT("Target"), Actor);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sense Out: %s"), *Actor->GetName());

		if (BlackboardComp)
		{
			BlackboardComp->ClearValue(TEXT("Target"));
		}
	}
}