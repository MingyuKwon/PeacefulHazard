// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyPichUpItem.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "Controller/HappyPlayerController.h"
#include "GameMode/PeaceFulHazardGameMode.h"

void AHappyPichUpItem::BeginPlay()
{
	Super::BeginPlay();

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->InteractWithItemUIEvent.AddDynamic(this, &ThisClass::CheckBroadCastItemIsMe);
	}
}

void AHappyPichUpItem::AfterInteraction()
{
	Super::AfterInteraction();

	DestroyItem();
}

void AHappyPichUpItem::CheckBroadCastItemIsMe(EItemType _itemtype, int32 _count)
{
	if (_itemtype == ItemType && _count == ItemCount)
	{
		if (bInteractWithUI)
		{
			if (PeaceFulHazardGameMode)
			{
				PeaceFulHazardGameMode->SetAleradyInteract(GetName());
			}

			bAlreadyInteract = true;
			AfterInteraction();

		}
	}
}

void AHappyPichUpItem::EnalbleCustomDepth(bool bEnable)
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetRenderCustomDepth(bEnable);
	}
	
}

void AHappyPichUpItem::InteractWithPlayer(APeaceFulHazardCharacter* character)
{
	AHappyPlayerController* playerController = Cast<AHappyPlayerController>(character->GetController());

	if (character)
	{
		bInteractWithUI = true;
		playerController->GetItem(ItemType, ItemCount);
	}
}

AHappyPichUpItem::AHappyPichUpItem()
{
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCustomDepthStencilValue(250);
	StaticMeshComponent->SetRenderCustomDepth(true);
}
