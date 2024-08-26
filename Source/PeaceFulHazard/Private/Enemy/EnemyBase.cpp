// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UI/PlayerHUD.h"
#include "Controller/HappyPlayerController.h"
#include "Battle/Weapon.h"
#include "Components/BoxComponent.h"
#include "Item/HappyInteractableItem.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DamageEvents.h"
#include "Materials/MaterialInstance.h"
#include "System/EnemyRoutePivot.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	HeadBox->SetupAttachment(RootComponent);
	HeadBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FX_Head"));

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bUseRVOAvoidance = true;
}

void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MatchYawWithController(DeltaTime);
}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (BehaviorTree && NewController)
	{
		EnemyAIController = Cast<AEnemyAIController>(NewController);
		if (EnemyAIController)
		{
			EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
			EnemyAIController->RunBehaviorTree(BehaviorTree);
		}
	}
}

void AEnemyBase::MatchYawWithController(float DeltaTime)
{
    FRotator CurrentRotation = GetActorRotation();

    FRotator ControlRotation = GetControlRotation();

    float NewYaw = FMath::FInterpTo(CurrentRotation.Yaw, ControlRotation.Yaw, DeltaTime, 5.f);

    FRotator NewRotation(0, NewYaw, 0);
    SetActorRotation(NewRotation);
}

void AEnemyBase::StopHeadStunMontage()
{
    if (HeadStunMontage)
    {
        GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, HeadStunMontage);
    }
}

void AEnemyBase::StopDamageStunMontage()
{
    if (DamageStunMontage)
    {
        GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, DamageStunMontage);
    }
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage <= 0.0f) return 0;

    FVector ShotDirection;
    bool bHead = false;
    if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
    {
        FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
        ShotDirection = PointDamageEvent->ShotDirection;

        bHead = PointDamageEvent->HitInfo.BoneName == "Head";

        if (bHead)
        {
            ActualDamage *= 2.0f; // 헤드샷의 경우 데미지 2배
            UE_LOG(LogTemp, Display, TEXT("Head Shot"));
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("Body Shot"));
        }

        UE_LOG(LogTemp, Warning, TEXT("Received Point Damage: %f from %s"), ActualDamage, *DamageCauser->GetName());
    }
    else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
    {
        // Radial damage handling code here (e.g., explosions)
    }
    else
    {
        // General damage handling code here
    }

    if (EnemyAIController)
    {
        EnemyAIController->EnemyTakeDamge(ActualDamage, bHead);

        if (EnemyAIController->bStunHeadShot)
        {
            if (HeadStunMontage)
            {
                PlayAnimMontage(HeadStunMontage);
            }
        }
        
        if (EnemyAIController->bStunDamage)
        {
            if (DamageStunMontage)
            {
                PlayAnimMontage(DamageStunMontage);
            }
        }


        if (EnemyAIController->bDeath)
        {
            GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));  // 충돌 설정을 Ragdoll로 변경
            GetCharacterMovement()->DisableMovement();  // 캐릭터의 모든 움직임 비활성화
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 캡슐 콜리전 비활성화

            SetLifeSpan(1.5f);

        }
        
    }

    PlayHitMontage(ShotDirection);

    // 여기에 bDeath true 이면 죽는 애니메이션 진행 하도록 하자

    return ActualDamage;
}

void AEnemyBase::PlayHitMontage(FVector ShotDirection)
{
    // Calculate the direction the shot came from relative to the enemy's forward direction
    FVector ForwardVector = GetActorForwardVector();
    FVector ToShotDirection = ShotDirection.GetSafeNormal();

    // Calculate the angle between the shot direction and the forward vector
    float Angle = FMath::Acos(FVector::DotProduct(ForwardVector, ToShotDirection)) * (180.0f / PI);

    // Determine the direction from which the shot came
    FVector CrossProduct = FVector::CrossProduct(ForwardVector, ToShotDirection);
    if (CrossProduct.Z < 0)
    {
        Angle = -Angle;
    }

    // Play appropriate hit montage based on the angle
    if (Angle >= -45.0f && Angle <= 45.0f)
    {
        // Back hit
        if (HitBackMontage)
        {
            PlayAnimMontage(HitBackMontage);
            UE_LOG(LogTemp, Display, TEXT("Hit from Back"));
        }

    }
    else if (Angle > 45.0f && Angle <= 135.0f)
    {
        // Left hit
        if (HitLeftMontage)
        {
            PlayAnimMontage(HitLeftMontage);
            UE_LOG(LogTemp, Display, TEXT("Hit from Left"));
        }
    }
    else if (Angle < -45.0f && Angle >= -135.0f)
    {
        // Right hit
        if (HitRightMontage)
        {
            PlayAnimMontage(HitRightMontage);
            UE_LOG(LogTemp, Display, TEXT("Hit from Right"));
        }
    }
    else
    {
        // Forward hit
        if (HitFwdMontage)
        {
            PlayAnimMontage(HitFwdMontage);
            UE_LOG(LogTemp, Display, TEXT("Hit from Front"));
        }
    }

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorld()->GetTimerManager().SetTimer(updateTimerHandle, this, &AEnemyBase::UpdateValue, 0.1f, true);
    FindEnemyRoutes();
}

void AEnemyBase::UpdateValue()
{
    if (EnemyAIController)
    {
        if (EnemyAIController->bDeath)
        {
            dissolvePercent += 0.09f;
        }
        else
        {
            dissolvePercent -= 0.09f;
        }

        dissolvePercent = FMath::Clamp(dissolvePercent, 0.f, 1.f);

        if (!(dissolvePercent <= 0 || dissolvePercent >= 1))
        {
            GetMesh()->SetScalarParameterValueOnMaterials("Dissolve", dissolvePercent);
        }
    }

}

void AEnemyBase::FindEnemyRoutes()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyRoutePivot::StaticClass(), FoundActors);

    AEnemyRoutePivot* ClosestPivot = nullptr;
    float MinDistance = FLT_MAX;

    for (AActor* Actor : FoundActors)
    {
        AEnemyRoutePivot* RoutePivot = Cast<AEnemyRoutePivot>(Actor);
        if (RoutePivot)
        {
            float Distance = FVector::Dist(GetActorLocation(), RoutePivot->GetActorLocation());
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                ClosestPivot = RoutePivot;
            }
        }
    }

    if (ClosestPivot)
    {
        ERouteNum TargetRouteNum = ClosestPivot->HitRightMontage;

        TArray<AEnemyRoutePivot*> EnemyRoutePivots;
        for (AActor* Actor : FoundActors)
        {
            AEnemyRoutePivot* RoutePivot = Cast<AEnemyRoutePivot>(Actor);
            if (RoutePivot && RoutePivot->HitRightMontage == TargetRouteNum)
            {
                EnemyRoutePivots.Add(RoutePivot);
            }
        }

        for (AEnemyRoutePivot* RoutePivot : EnemyRoutePivots)
        {
            // 필요한 작업 수행
        }

        if (EnemyAIController)
        {
            EnemyAIController->TriggerRoute(EnemyRoutePivots);
        }
    }
}


// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



