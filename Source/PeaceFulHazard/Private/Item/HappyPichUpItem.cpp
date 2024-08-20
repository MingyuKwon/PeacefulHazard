// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyPichUpItem.h"
#include "Character/PeaceFulHazardCharacter.h"

void AHappyPichUpItem::InteractWithPlayer(APeaceFulHazardCharacter* character)
{
	switch (ItemType)
	{
	case EItemType::EIT_Bullet_Noraml :

		break;
	}

	if (character)
	{
		character->ChangeItemInventory(ItemType, ItemCount);
	}

	DestroyItem();
}
