// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyInteractableSituation.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "Controller/HappyPlayerController.h"
#include "GameMode/PeaceFulHazardGameMode.h"

void AHappyInteractableSituation::BeginPlay()
{
	Super::BeginPlay();

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->InteractSituationEvent.AddDynamic(this, &ThisClass::CheckBroadCastItemIsMe);
	}

}

void AHappyInteractableSituation::AfterInteraction()
{
	Super::AfterInteraction();

	switch (situationType)
	{
	case EInteractSituationType::EIST_NormalDoor:
		if (AdditiveStaticMesh1)
		{
			AdditiveStaticMesh1->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		}

		if (AdditiveStaticMesh2)
		{
			AdditiveStaticMesh2->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		}

		break;
	}
}

void AHappyInteractableSituation::CheckBroadCastItemIsMe(EInteractSituationType _situationType)
{
	if (_situationType != situationType) return;
	if (!bInteractWithUI) return;

	AfterInteraction();
}

void AHappyInteractableSituation::InteractWithPlayer(APeaceFulHazardCharacter* character)
{
	AHappyPlayerController* playerController = Cast<AHappyPlayerController>(character->GetController());

	if (playerController)
	{
		bInteractWithUI = true;

		if (situationType == EInteractSituationType::EIST_ItemBox)
		{
			playerController->TriggerItemBox();
		}
		else
		{
			playerController->TriggerSituation(situationType);
		}
	}
}

AHappyInteractableSituation::AHappyInteractableSituation()
{
	AdditiveStaticMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AdditiveStaticMesh1"));
	AdditiveStaticMesh1->SetupAttachment(StaticMeshComponent);

	AdditiveStaticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AdditiveStaticMesh2"));
	AdditiveStaticMesh2->SetupAttachment(StaticMeshComponent);

}
