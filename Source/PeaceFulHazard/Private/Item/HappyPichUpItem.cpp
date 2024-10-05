// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyPichUpItem.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "Controller/HappyPlayerController.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"


void AHappyPichUpItem::CheckToShow()
{
	CurrentWaveCount++;

	if (CurrentWaveCount == WaveCount)
	{
		if (PeaceFulHazardGameMode)
		{
			PeaceFulHazardGameMode->DynamicSpawnStartEvent.RemoveDynamic(this, &ThisClass::CheckToShow);
		}

		SetActorVisibility(true);
	}
}

void AHappyPichUpItem::BeginPlay()
{
	Super::BeginPlay();

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->InteractWithItemUIEvent.AddDynamic(this, &ThisClass::CheckBroadCastItemIsMe);

		switch (PeaceFulHazardGameMode->GetDifficulty())
		{
		case EDifficulty::ED_Easy:
			ItemCount = EasyItemCount;
			break;

		case EDifficulty::ED_Normal:
			ItemCount = NormalItemCount;
			break;

		case EDifficulty::ED_Hard:
			ItemCount = HardItemCount;
			break;
		}
		
		if (bFinalBattle )
		{
			SetActorVisibility(false);

			if (PeaceFulHazardGameMode->GetPlayerToDo() == EPlayerToDo::EPTD_Survive)
			{
				PeaceFulHazardGameMode->DynamicSpawnStartEvent.AddDynamic(this, &ThisClass::CheckToShow);

			}

		}
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
				switch (ItemType)
				{
				case EItemType::EIT_GraveYardDoorKey:
					PeaceFulHazardGameMode->ToDoUpdate(EPlayerToDo::EPTD_SearchGravetard);
					break;
				case EItemType::EIT_CrossOverFirstDoorKey:
					PeaceFulHazardGameMode->ToDoUpdate(EPlayerToDo::EPTD_SearchRightGarden);
					break;
				case EItemType::EIT_MainCatheralDoorKey:
					PeaceFulHazardGameMode->ToDoUpdate(EPlayerToDo::EPTD_SearchCathedral);
					break;
				case EItemType::EIT_CrossOverSecondDoorKey:
					PeaceFulHazardGameMode->ToDoUpdate(EPlayerToDo::EPTD_SearchLeftGarden);
					break;
				case EItemType::EIT_MainCatheralDoorKey2:
					PeaceFulHazardGameMode->ToDoUpdate(EPlayerToDo::EPTD_SearchCathedralSecondFloor);
					break;
				case EItemType::EIT_CathedralLockKey:
					PeaceFulHazardGameMode->ToDoUpdate(EPlayerToDo::EPTD_GetTreasure);
					break;
				case EItemType::EIST_Treasure:
					PeaceFulHazardGameMode->ToDoUpdate(EPlayerToDo::EPTD_Survive);
					break;


				}
				PeaceFulHazardGameMode->UpdateDefaultUIEvent.Broadcast();

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
