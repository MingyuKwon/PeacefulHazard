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
#include "Character/PeaceFulHazardCharacter.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	HeadBox->SetupAttachment(RootComponent);

    AttackRangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackRangeBox"));
    AttackRangeBox->SetupAttachment(RootComponent);

    
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    HeadBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    AttackRangeBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

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

void AEnemyBase::Attack()
{
    if (AttackMontage)
    {
        GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
    }
}

void AEnemyBase::AttackImpact(int32 index)
{
    TArray<AActor*> OverlappingActors;
    AttackRangeBox->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        APeaceFulHazardCharacter* TargetCharacter = Cast<APeaceFulHazardCharacter>(Actor);
        if (TargetCharacter)
        {
            UGameplayStatics::ApplyDamage(TargetCharacter, EnemyDamageAmount, GetController(), this, UDamageType::StaticClass());
        }
    }
}

void AEnemyBase::AttackEnd()
{
    if (EnemyAIController)
    {
        EnemyAIController->bNowAttacking = false;
    }
}

void AEnemyBase::PlayAttackSound(int32 index)
{
    if (PeaceFulHazardGameMode)
    {
        PeaceFulHazardGameMode->PlaySoundInGameplay(AttackSound, GetActorLocation(), 1.f);
    }
}

void AEnemyBase::MatchYawWithController(float DeltaTime)
{
    float CurrentYaw = GetActorRotation().Yaw;

    float ControlYaw = GetControlRotation().Yaw;

    float DeltaYaw = FMath::UnwindDegrees(ControlYaw - CurrentYaw);

    float NewYaw = CurrentYaw + FMath::Lerp(0.0f, DeltaYaw, DeltaTime * 5.f);

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

            if (PeaceFulHazardGameMode)
            {
                PeaceFulHazardGameMode->PlaySoundInGameplay(HeadHitSound, GetActorLocation(), 1.3f);
            }
        }
        else
        {
            if (PeaceFulHazardGameMode)
            {
                PeaceFulHazardGameMode->PlaySoundInGameplay(HitSound, GetActorLocation(), 1.f);
            }
        }
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
                StopAnimMontage(AttackMontage);
                AttackEnd();
                PlayAnimMontage(HeadStunMontage);
            }
        }
        
        if (EnemyAIController->bStunDamage)
        {
            if (DamageStunMontage)
            {
                StopAnimMontage(AttackMontage);
                AttackEnd();
                PlayAnimMontage(DamageStunMontage);
            }
        }


        if (EnemyAIController->bDeath)
        {
            GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));  // 충돌 설정을 Ragdoll로 변경
            GetCharacterMovement()->DisableMovement();  // 캐릭터의 모든 움직임 비활성화
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 캡슐 콜리전 비활성화

            if (PeaceFulHazardGameMode)
            {
                PeaceFulHazardGameMode->SetAleradyInteract(GetName());
                PeaceFulHazardGameMode->SetEnemyRefCount(false);
                PeaceFulHazardGameMode->PlaySoundInGameplay(DeathSound, GetActorLocation(), 1.f);
            }

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
        }

    }
    else if (Angle > 45.0f && Angle <= 135.0f)
    {
        // Left hit
        if (HitLeftMontage)
        {
            PlayAnimMontage(HitLeftMontage);
        }
    }
    else if (Angle < -45.0f && Angle >= -135.0f)
    {
        // Right hit
        if (HitRightMontage)
        {
            PlayAnimMontage(HitRightMontage);
        }
    }
    else
    {
        // Forward hit
        if (HitFwdMontage)
        {
            PlayAnimMontage(HitFwdMontage);
        }
    }

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
    PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (PeaceFulHazardGameMode)
    {
        if (PeaceFulHazardGameMode->CheckAleradyInteract(GetName()))
        {
            Destroy();
            return;
        }
        else
        {
            PeaceFulHazardGameMode->SetEnemyRefCount(true);
            PeaceFulHazardGameMode->MapEndEvent.AddDynamic(this, &ThisClass::MapEndCallBack);
            PeaceFulHazardGameMode->MapStartEvent.AddDynamic(this, &ThisClass::MapStartCallBack);      
            PeaceFulHazardGameMode->WantSaveEvent.AddDynamic(this, &ThisClass::WantToSaveCallBack);

        }
    }

    GetWorld()->GetTimerManager().SetTimer(updateTimerHandle, this, &AEnemyBase::UpdateValue, 0.1f, true);
    FindEnemyRoutes();

    HeadBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FX_Head"));


}


void AEnemyBase::MapEndCallBack()
{
    if (EnemyAIController)
    {
        PeaceFulHazardGameMode->SaveEnemyStats(GetName(), EnemyAIController->currentHealth, GetActorLocation(), GetActorRotation());
    }
        
    PeaceFulHazardGameMode->SetEnemyRefCount(false);
}

void AEnemyBase::MapStartCallBack()
{
    float enemyHealth = 0;
    FVector enemyLocation = GetActorLocation();
    FRotator enemyRotation = GetActorRotation();

    if (PeaceFulHazardGameMode->GetEnemyStats(GetName(), enemyHealth, enemyLocation, enemyRotation))
    {
        if (EnemyAIController)
        {
            EnemyAIController->currentHealth = enemyHealth;
            SetActorLocation(enemyLocation);
            SetActorRotation(enemyRotation);
        }
    }

}

void AEnemyBase::WantToSaveCallBack()
{
    if (EnemyAIController)
    {
        PeaceFulHazardGameMode->SaveEnemyStats(GetName(), EnemyAIController->currentHealth, GetActorLocation(), GetActorRotation());
    }

    PeaceFulHazardGameMode->SetEnemySaveRefCount(false);

}

void AEnemyBase::UpdateValue()
{
    if (EnemyAIController)
    {
        if (EnemyAIController->Target || EnemyAIController->bFollowingLastPositon)
        {
            GetCharacterMovement()->MaxWalkSpeed = ChasingMoveSpeed;
        }
        else
        {
            GetCharacterMovement()->MaxWalkSpeed = PatrolMoveSpeed;
        }


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

    ERouteNum TargetRouteNum = routeNum;

    TArray<AEnemyRoutePivot*> EnemyRoutePivots;
    for (AActor* Actor : FoundActors)
    {
        AEnemyRoutePivot* RoutePivot = Cast<AEnemyRoutePivot>(Actor);
        if (RoutePivot && RoutePivot->HitRightMontage == TargetRouteNum)
        {
            EnemyRoutePivots.Add(RoutePivot);
        }
    }

    if (EnemyAIController)
    {
        EnemyAIController->TriggerRoute(EnemyRoutePivots);
    }

    
}


// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



