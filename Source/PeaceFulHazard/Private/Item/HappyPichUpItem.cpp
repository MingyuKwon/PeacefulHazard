// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyPichUpItem.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "Controller/HappyPlayerController.h"

void AHappyPichUpItem::InteractWithPlayer(APeaceFulHazardCharacter* character)
{
	switch (ItemType)
	{
	case EItemType::EIT_Bullet_Noraml :

		break;
	}

	AHappyPlayerController* playerController = Cast<AHappyPlayerController>(character->GetController());

	if (character)
	{
		playerController->GetItem(ItemType, ItemCount);
	}

	DestroyItem();
}
