// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "Enemy/EnemyBase.h"
#include "DrawDebugHelpers.h"
#include "System/EnemyRoutePivot.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	currentHealth = FMath::Clamp(currentHealth, 0, currentHealth);

	damageAccumulate += Damage;

	if (stundamageAccumulateUnit <= damageAccumulate)
	{
		damageAccumulate = 0;
		bStunDamage = true;
		GetWorld()->GetTimerManager().SetTimer(DamgeStunTimerHandle, this, &ThisClass::DamageStunRelease, 2.f, false);
	}

	if (bHead)
	{
		bStunHeadShot = true;
		GetWorld()->GetTimerManager().SetTimer(HeadStunTimerHandle, this, &ThisClass::HeadStunRelease, 0.7f, false);
	}

	if (currentHealth <= 0)
	{
		bDeath = true;
	}
}

void AEnemyAIController::TriggerRoute(TArray<AEnemyRoutePivot*> Pivots)
{
	Pivots.Sort([](const AEnemyRoutePivot& A, const AEnemyRoutePivot& B)
		{
			return A.index < B.index;
		});

	RoutePivots = Pivots;

	TriggerResetPivotIndex(false);
}

void AEnemyAIController::TriggerResetPivotIndex(bool bFollowingLastPosition)
{
	UE_LOG(LogTemp, Warning, TEXT("TriggerResetPivotIndex"));

	if (bFollowingLastPosition)
	{
		if (GetPawn() && RoutePivots.Num() > 0)
		{
			FVector PawnLocation = GetPawn()->GetActorLocation();
			float MinDistance = FLT_MAX;
			int32 ClosestIndex = 0;

			for (int32 i = 0; i < RoutePivots.Num(); i++)
			{
				float Distance = FVector::Dist(PawnLocation, RoutePivots[i]->GetActorLocation());
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					ClosestIndex = i;
				}
			}

			routeIndex = ClosestIndex;
			TargetLocation = RoutePivots[routeIndex]->GetActorLocation();

			bFollowingLastPositon = false;
		}
	}
	else
	{
		routeIndex++;
		if (routeIndex == RoutePivots.Num()) routeIndex = 0;

		routeIndex = FMath::Clamp(routeIndex, 0, RoutePivots.Num() - 1);

		TargetLocation = RoutePivots[routeIndex]->GetActorLocation();
	}
}

bool AEnemyAIController::CheckMovetoDestination()
{
	if (GetPawn() == nullptr ) return false;
	FVector PawnPosition = GetPawn()->GetActorLocation();

	float distance = FVector::Dist2D(PawnPosition, TargetLocation);

	UE_LOG(LogTemp, Warning, TEXT("%s, %f"), *GetName(), distance);


	return distance <= 40.f;
}

void AEnemyAIController::UpdateBlackBoard()
{
	if (Target)
	{
		TargetLocation = Target->GetActorLocation();

	}
	else
	{
		if (CheckMovetoDestination())
		{
			TriggerResetPivotIndex(bFollowingLastPositon);
		}
	}

	DrawDebugSphere(GetWorld(), TargetLocation, 100.f, 30, FColor::Blue, false, 0.1f);

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("bDeath"), bDeath);
		BlackboardComp->SetValueAsBool(TEXT("bStunDamage"), bStunDamage);
		BlackboardComp->SetValueAsBool(TEXT("bStunHeadShot"), bStunHeadShot);
		BlackboardComp->SetValueAsVector(TEXT("TargetLocation"), TargetLocation);
		BlackboardComp->SetValueAsObject(TEXT("Target"), Target);


	}
}

void AEnemyAIController::HeadStunRelease()
{
	if (bDeath) return;

	bStunHeadShot = false;
	if (controlEnemy)
	{
		controlEnemy->StopHeadStunMontage();
	}
}

void AEnemyAIController::DamageStunRelease()
{
	if (bDeath) return;

	bStunDamage = false;
	if (controlEnemy)
	{
		controlEnemy->StopDamageStunMontage();
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	
	BlackboardComp = GetBlackboardComponent();
	GetWorld()->GetTimerManager().SetTimer(updateTimerHandle, this, &AEnemyAIController::UpdateBlackBoard , 0.1f, true);

	controlEnemy = Cast<AEnemyBase>(GetPawn());
		
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

		bFollowingLastPositon = false;
		Target = Cast<APeaceFulHazardCharacter>(Actor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sense Out: %s"), *Actor->GetName());
		bFollowingLastPositon = true;
		Target = nullptr;

	}
}