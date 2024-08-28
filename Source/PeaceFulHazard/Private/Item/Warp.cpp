// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Warp.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "Controller/HappyPlayerController.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Components/BoxComponent.h"

void AWarp::BeginPlay()
{
	Super::BeginPlay();

}

void AWarp::AfterInteraction()
{

}

void AWarp::InteractWithPlayer(APeaceFulHazardCharacter* character)
{
	AHappyPlayerController* playerController = Cast<AHappyPlayerController>(character->GetController());

	if (playerController)
	{
		if (PeaceFulHazardGameMode)
		{
			PeaceFulHazardGameMode->MapTravelEvent.Broadcast(WarpTarget);
		}
	}
}

AWarp::AWarp()
{
	AdditiveStaticMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AdditiveStaticMesh1"));
	AdditiveStaticMesh1->SetupAttachment(StaticMeshComponent);

	AdditiveStaticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AdditiveStaticMesh2"));
	AdditiveStaticMesh2->SetupAttachment(StaticMeshComponent);

}
