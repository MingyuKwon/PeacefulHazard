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

