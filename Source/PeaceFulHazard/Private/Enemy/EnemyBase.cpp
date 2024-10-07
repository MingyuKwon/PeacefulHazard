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
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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

    SpawnNiagaraPoint = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpawnNiagaraPoint"));
    SpawnNiagaraPoint->SetupAttachment(RootComponent);


    stundamageAccumulateUnit.Easy = 40.f;
    stundamageAccumulateUnit.Normal = 70.f;
    stundamageAccumulateUnit.Hard = 100.f;


    stunHeadDamageAccumulateUnit.Easy = 20.f;
    stunHeadDamageAccumulateUnit.Normal = 35.f;
    stunHeadDamageAccumulateUnit.Hard = 50.f;


    MaxHealth.Easy = 100;
    MaxHealth.Normal = 150;
    MaxHealth.Hard = 250;

    EnemyDamageAmount.Easy = 10;
    EnemyDamageAmount.Normal = 15;
    EnemyDamageAmount.Hard = 20;
}

float AEnemyBase::GetDamageUnitbyDifficulty()
{
    if (PeaceFulHazardGameMode == nullptr) return 0.f;

    switch (PeaceFulHazardGameMode->GetDifficulty())
    {
    case EDifficulty::ED_Easy:
        return stundamageAccumulateUnit.Easy;
        break;

    case EDifficulty::ED_Normal:
        return stundamageAccumulateUnit.Normal;
        break;

    case EDifficulty::ED_Hard:
        return stundamageAccumulateUnit.Hard;
        break;
    }

    return 0.f;
}

float AEnemyBase::GetHeadUnitbyDifficulty()
{
    if (PeaceFulHazardGameMode == nullptr) return 0.f;

    switch (PeaceFulHazardGameMode->GetDifficulty())
    {
    case EDifficulty::ED_Easy:
        return stunHeadDamageAccumulateUnit.Easy;
        break;

    case EDifficulty::ED_Normal:
        return stunHeadDamageAccumulateUnit.Normal;

        break;

    case EDifficulty::ED_Hard:
        return stunHeadDamageAccumulateUnit.Hard;

        break;
    }

    return 0.f;
}

float AEnemyBase::GetMaxHealthbyDifficulty()
{
    if (PeaceFulHazardGameMode == nullptr) return 0.f;

    switch (PeaceFulHazardGameMode->GetDifficulty())
    {
    case EDifficulty::ED_Easy:
        return MaxHealth.Easy;
        break;

    case EDifficulty::ED_Normal:
        return MaxHealth.Normal;

        break;

    case EDifficulty::ED_Hard:
        return MaxHealth.Hard;

        break;
    }

    return 0.f;
}

float AEnemyBase::GetEnemyDamageAmountbyDifficulty()
{
    if (PeaceFulHazardGameMode == nullptr) return 0.f;

    switch (PeaceFulHazardGameMode->GetDifficulty())
    {
    case EDifficulty::ED_Easy:
        return EnemyDamageAmount.Easy;
        break;

    case EDifficulty::ED_Normal:
        return EnemyDamageAmount.Normal;

        break;

    case EDifficulty::ED_Hard:
        return EnemyDamageAmount.Hard;

        break;
    }

    return 0.f;
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

void AEnemyBase::Attack(bool bBossRange)
{
    if (bBossRange)
    {
        if (AttackMontage2)
        {
            GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage2);
        }

        return;
    }

    int32 RandomChoice = FMath::RandRange(0, 9);

    if (bBoss && RandomChoice <= 6 || !bBoss && RandomChoice <= 4)
    {
        if (AttackMontage)
        {
            GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
        }
    }
    else
    {
        if (AttackMontage2)
        {
            GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage2);
        }
    }
}

void AEnemyBase::AttackImpact(int32 index)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    if (bBoss && AnimInstance && AnimInstance->Montage_IsPlaying(AttackMontage2))
    {
        if (BeforeBombEffect && EnemyAIController && AfterBombEffect)
        {
            BombLocationSave = EnemyAIController->TargetLocation;
            FHitResult HitResult;

            // 라인 트레이스 실행 (아래로 Raycast)
            bool bHit = GetWorld()->LineTraceSingleByChannel(
                HitResult,            
                BombLocationSave,       
                BombLocationSave + FVector::DownVector * 10000.f,          
                ECC_Visibility      
            );

            if (bHit)
            {
                BombLocationSave = HitResult.ImpactPoint;
            }


            UNiagaraComponent* bombBeforeNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                BeforeBombEffect,
                BombLocationSave
            );

            FTimerHandle bombTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(bombTimerHandle, [this]() {

                UNiagaraComponent* bombAfterNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    GetWorld(),
                    AfterBombEffect,
                    BombLocationSave + FVector::UpVector * 50.f
                );

                float InnerRadius = 50.f;
                float OuterRadius = 210.f;

                UGameplayStatics::ApplyRadialDamageWithFalloff(
                    GetWorld(),
                    GetEnemyDamageAmountbyDifficulty(),          // 최대 데미지
                    10.f,                       // 최소 데미지
                    BombLocationSave + FVector::UpVector * 50.f, // 폭탄 위치
                    InnerRadius,                      // 내부 반경: 최대 데미지를 가할 반경
                    OuterRadius,                      // 외부 반경: 최소 데미지를 가할 반경
                    1.0f,                       // 데미지 감소 시작 거리와 끝 거리 사이의 선형 데미지 감소 계수
                    nullptr,                    // 데미지 타입 클래스 (기본값)
                    TArray<AActor*>(),          // 피해 제외 대상 (없음)
                    this,                       // 피해를 입히는 액터 (자기 자신)
                    GetInstigatorController(),  // 공격자의 컨트롤러
                    ECC_Visibility              // 데미지 적용 채널 (Visibility)
                );
                }, 1.f, false);
        }
    }
    else
    {
        TArray<AActor*> OverlappingActors;
        AttackRangeBox->GetOverlappingActors(OverlappingActors);

        for (AActor* Actor : OverlappingActors)
        {
            APeaceFulHazardCharacter* TargetCharacter = Cast<APeaceFulHazardCharacter>(Actor);
            if (TargetCharacter)
            {
                UGameplayStatics::ApplyDamage(TargetCharacter, GetEnemyDamageAmountbyDifficulty(), GetController(), this, UDamageType::StaticClass());
            }
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

bool AEnemyBase::ShowChasingNiagara()
{
    if (SpawnNiagaraPoint == nullptr) return false;

    if (SpawnNiagaraPoint->IsActive())
    {
        return false;
    }
    else
    {
        SpawnNiagaraPoint->Activate();
        return true;
    }
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
    if (Cast<AEnemyBase>(DamageCauser) != nullptr) return 0 ;

    FVector ShotDirection;
    bool bHead = false;
    if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
    {
        FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
        ShotDirection = PointDamageEvent->ShotDirection;

        bHead = PointDamageEvent->HitInfo.BoneName == "Head";

        if (bHead)
        {
            ActualDamage *= 1.5f;

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
                StopAnimMontage(AttackMontage2);

                AttackEnd();
                PlayAnimMontage(HeadStunMontage);
            }
        }
        
        if (EnemyAIController->bStunDamage)
        {
            if (DamageStunMontage)
            {
                StopAnimMontage(AttackMontage);
                StopAnimMontage(AttackMontage2);
                AttackEnd();
                PlayAnimMontage(DamageStunMontage);
            }
        }


        if (EnemyAIController->bDeath)
        {
            GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));  // �浹 ������ Ragdoll�� ����
            GetCharacterMovement()->DisableMovement();  // ĳ������ ��� ������ ��Ȱ��ȭ
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // ĸ�� �ݸ��� ��Ȱ��ȭ

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

    // ���⿡ bDeath true �̸� �״� �ִϸ��̼� ���� �ϵ��� ����

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

void AEnemyBase::SetMaterialParaLerp(bool bDissolve, float value)
{
    if (bDissolve)
    {
        bool bReduce = value < dissolvePercent;

        dissolvePercent = FMath::Lerp(dissolvePercent, value, bReduce ? 0.1f : 0.2f);
        GetMesh()->SetScalarParameterValueOnMaterials("Dissolve", dissolvePercent);

    }
    else
    {
        bool bReduce = value < StunPercent;

        StunPercent = FMath::Lerp(StunPercent, value, bReduce ? 0.5f : 0.8f);
        GetMesh()->SetScalarParameterValueOnMaterials("Stun", StunPercent);

    }
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
    PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (PeaceFulHazardGameMode)
    {
        // If it is not time to enemy spawn
        if (PeaceFulHazardGameMode->GetPlayerToDo() < spawnProgress)
        {
            Destroy();
            return;
        }


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

    if (EnemyAIController)
    {
        EnemyAIController->currentHealth = GetMaxHealthbyDifficulty();

    }

    GetWorld()->GetTimerManager().SetTimer(updateTimerHandle, this, &AEnemyBase::UpdateValue, 0.1f, true);
    FindEnemyRoutes();

    HeadBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FX_Head"));

    if (SpawnNiagaraPoint)
    {
        SpawnNiagaraPoint->Deactivate();
    }



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
    else
    {
        EnemyAIController->currentHealth = GetMaxHealthbyDifficulty();
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
        float plusSpeed = 20.f;

        if (EnemyAIController->Target || EnemyAIController->bFollowingLastPositon)
        {
            GetCharacterMovement()->MaxWalkSpeed = ChasingMoveSpeed + plusSpeed;
        }
        else
        {
            GetCharacterMovement()->MaxWalkSpeed = PatrolMoveSpeed + plusSpeed;
        }

        if (EnemyAIController->bStunDamage || EnemyAIController->bStunHeadShot && !EnemyAIController->bDeath)
        {
            SetMaterialParaLerp(false, 0.25f);
        }
        else
        {
            SetMaterialParaLerp(false, 0.f);

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



