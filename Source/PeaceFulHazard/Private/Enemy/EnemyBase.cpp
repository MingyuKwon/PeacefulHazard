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

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	HeadBox->SetupAttachment(RootComponent);
	HeadBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FX_Head"));



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

void AEnemyBase::StopHeadStunMontage()
{
    if (HeadStunMontage)
    {
        GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, HeadStunMontage);
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
            if (EnemyAIController)
            {
                EnemyAIController->UnPossess(); 
            }

            GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));  // 충돌 설정을 Ragdoll로 변경
            GetCharacterMovement()->DisableMovement();  // 캐릭터의 모든 움직임 비활성화
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 캡슐 콜리전 비활성화

            SetLifeSpan(2.f);

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
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



