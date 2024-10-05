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
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h"

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

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (controlEnemy && controlEnemy->bBoss && !bNowAttacking && !bDeath && !bStunDamage && !bStunHeadShot)
	{
		FVector DirectionToTarget = (TargetLocation - controlEnemy->GetActorLocation()).GetSafeNormal();
		FRotator TargetRotation = DirectionToTarget.Rotation();

		float DeltaTime = GetWorld()->GetDeltaSeconds();
		FRotator NewRotation = FMath::RInterpTo(controlEnemy->GetActorRotation(), TargetRotation, DeltaTime, 2.5f);

		controlEnemy->SetActorRotation(NewRotation);

	}
}

void AEnemyAIController::EnemyTakeDamge(float Damage, bool bHead)
{
	if (controlEnemy == nullptr) return;

	currentHealth -= Damage;
	currentHealth = FMath::Clamp(currentHealth, 0, currentHealth);

	damageAccumulate += Damage;

	if (controlEnemy->GetDamageUnitbyDifficulty() <= damageAccumulate)
	{
		damageAccumulate = 0;
		bStunDamage = true;
		GetWorld()->GetTimerManager().SetTimer(DamgeStunTimerHandle, this, &ThisClass::DamageStunRelease, 2.5f, false);
	}

	if (bHead)
	{
		headDamageAccumulate += Damage;

		if (controlEnemy->GetHeadUnitbyDifficulty() <= headDamageAccumulate)
		{
			headDamageAccumulate = 0;
			bStunHeadShot = true;
			GetWorld()->GetTimerManager().SetTimer(HeadStunTimerHandle, this, &ThisClass::HeadStunRelease, 0.9f, false);
		}

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

	TriggerResetPivotIndex(true);
}

void AEnemyAIController::TriggerResetPivotIndex(bool bFollowingLastPosition)
{
	if (RoutePivots.Num() <= 0) return;
	if (Target != nullptr) return;

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

void AEnemyAIController::GameClear()
{
	bGameClear = true;
}

void AEnemyAIController::PlayerDeathCallback()
{
	Target = nullptr;
	bFollowingLastPositon = false;
	TriggerResetPivotIndex(bFollowingLastPositon);

}

void AEnemyAIController::Attack(bool bBossRange)
{
	bNowAttacking = true;

	if (controlEnemy)
	{
		controlEnemy->Attack(bBossRange);
	}
}

bool AEnemyAIController::CheckMovetoDestination()
{
	if (GetPawn() == nullptr ) return false;
	FVector PawnPosition = GetPawn()->GetActorLocation();

	float distance = FVector::Dist2D(PawnPosition, TargetLocation);

	if (controlEnemy == nullptr)
	{
		controlEnemy = Cast<AEnemyBase>(GetPawn());
	}

	if (controlEnemy)
	{

		if (controlEnemy->bBoss)
		{
			FVector DirectionToTarget = (TargetLocation - controlEnemy->GetActorLocation()).GetSafeNormal();

			float DotProduct = FVector::DotProduct(controlEnemy->GetActorForwardVector(), DirectionToTarget);
			float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

			float AllowedAngle = 20.0f; // 허용되는 각도
			return AngleDegrees <= AllowedAngle;

		}
		else
		{
			return distance <= ((Target == nullptr) ? controlEnemy->PatrolMoveToRange : controlEnemy->AttackRange);
		}

	}

	return false;
}

void AEnemyAIController::UpdateBlackBoard()
{
	if (Target)
	{
		TargetLocation = Target->GetActorLocation();

		if (CheckMovetoDestination())
		{

			if (!bNowAttacking && !bDeath && !bStunDamage && !bStunHeadShot)
			{
				if (nonAttackLock) return;

				if (controlEnemy->bBoss)
				{
					float distance = FVector::Dist2D(GetPawn()->GetActorLocation(), TargetLocation);
					Attack(controlEnemy->AttackRange < distance);
				}
				else
				{
					Attack();
				}

			}
		}

		Target->ShowChasingNiagara();
		if (controlEnemy)
		{
			controlEnemy->ShowChasingNiagara();
		}
	}
	else
	{
		if (CheckMovetoDestination())
		{
			TriggerResetPivotIndex(bFollowingLastPositon);
		}

	}

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("bDeath"), bDeath);
		BlackboardComp->SetValueAsBool(TEXT("bStunDamage"), bStunDamage);
		BlackboardComp->SetValueAsBool(TEXT("bStunHeadShot"), bStunHeadShot);
		BlackboardComp->SetValueAsBool(TEXT("bNowAttacking"), bNowAttacking);

		BlackboardComp->SetValueAsVector(TEXT("TargetLocation"), TargetLocation);
		BlackboardComp->SetValueAsObject(TEXT("Target"), Target);

		BlackboardComp->SetValueAsBool(TEXT("bGameClear"), bGameClear);

		
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

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(this));

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayerDeathEvent.AddDynamic(this, &ThisClass::PlayerDeathCallback);
		PeaceFulHazardGameMode->GameClearEvent.AddDynamic(this, &ThisClass::GameClear);

		if (PeaceFulHazardGameMode->GetPlayerToDo() == EPlayerToDo::EPTD_Survive && PeaceFulHazardGameMode->currentMapType ==EWarpTarget::EWT_MainHub)
		{
			bSurviveMode = true;

			TArray<AActor*> FoundCharacters;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APeaceFulHazardCharacter::StaticClass(), FoundCharacters);

			if (FoundCharacters.Num() > 0)
			{
				Target = Cast<APeaceFulHazardCharacter>(FoundCharacters[0]);
			}

		}
	}

	GetWorld()->GetTimerManager().SetTimer(nonAttackTimerHandle, [this]() {
		nonAttackLock = false;
		}, bSurviveMode ? 1.f : 3.f, false);


		
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (bSurviveMode) return;

	for (AActor* Actor : UpdatedActors)
	{
		if (Actor->IsA(APeaceFulHazardCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player detected: %s"), *Actor->GetName());
		}
	}
}

// only visual perception will set target object, other perception will bring to impact point
void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	if (!Actor->IsA(APeaceFulHazardCharacter::StaticClass())) return;
	if (bSurviveMode) return;

	APeaceFulHazardCharacter* InputTarget = Cast<APeaceFulHazardCharacter>(Actor);
	if (InputTarget->bDeath) return;

	if (Stimulus.Type.Name == "Default__AISense_Sight")
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Sight Sense In: %s"), *Actor->GetName());

			bFollowingLastPositon = false;
			Target = InputTarget;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Sight Sense Out: %s"), *Actor->GetName());

			if (!bFollowingLastPositon)
			{
				bFollowingLastPositon = true;
				Target = nullptr;
			}
		}
	}
	else if (Stimulus.Type.Name == "Default__AISense_Damage")
	{
		if (Target != nullptr) return; // if enemy is targetting, does not nees anymore

		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Damage Sense In: %s"), *Actor->GetName());

			bFollowingLastPositon = true;
			TargetLocation = InputTarget->GetActorLocation();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Damage Sense Out: %s"), *Actor->GetName());

		}
	}
	else if (Stimulus.Type.Name == "Default__AISense_Hearing")
	{
		if (Target != nullptr) return;  // if enemy is targetting, does not nees anymore

		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hearing Sense In: %s"), *Actor->GetName());

			bFollowingLastPositon = true;
			TargetLocation = InputTarget->GetActorLocation();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hearing Sense Out: %s"), *Actor->GetName());


		}
	}
}
