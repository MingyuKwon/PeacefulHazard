// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyInteractableSituation.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "Controller/HappyPlayerController.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Components/BoxComponent.h"

void AHappyInteractableSituation::BeginPlay()
{
	Super::BeginPlay();

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->InteractSituationEvent.AddDynamic(this, &ThisClass::CheckBroadCastItemIsMe);


		if (situationType == EInteractSituationType::EIST_RedTriggerDoor ||
			situationType == EInteractSituationType::EIST_BlueTriggerDoor || 
			situationType == EInteractSituationType::EIST_GreenTriggerDoor ||
			situationType == EInteractSituationType::EIST_YellowTriggerDoor)
		{
			PeaceFulHazardGameMode->TriggerDoorEvent.AddDynamic(this, &ThisClass::ListenTirggerOn);
		}
	}

}

void AHappyInteractableSituation::ListenTirggerOn(EInteractSituationType _situationType)
{
	if ((situationType == EInteractSituationType::EIST_RedTriggerDoor && _situationType == EInteractSituationType::EIST_RedTrigger) || 
		(situationType == EInteractSituationType::EIST_GreenTriggerDoor && _situationType == EInteractSituationType::EIST_GreenTrigger) ||
		(situationType == EInteractSituationType::EIST_YellowTriggerDoor && _situationType == EInteractSituationType::EIST_YellowTrigger))
	{
		if (PeaceFulHazardGameMode)
		{
			FString string = FString("Some Door placed in this map opened!");
			PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(true, string);
			PeaceFulHazardGameMode->SetAleradyInteract(GetName());

			PeaceFulHazardGameMode->PlayUISound(InteractSuccessSound, 1.f);

		}

		AfterInteraction();

	}

	if (situationType == EInteractSituationType::EIST_BlueTriggerDoor && _situationType == EInteractSituationType::EIST_BlueTrigger)
	{
		if (PeaceFulHazardGameMode)
		{
			FString string = FString("To Take the key, you should trigger the blue points in this floor");

			if (PeaceFulHazardGameMode->currentMapType == EWarpTarget::EWT_MainCathedral)
			{
				string = FString("Now you can get the key!");
			}

			PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(true, string);
			PeaceFulHazardGameMode->SetAleradyInteract(GetName());

			PeaceFulHazardGameMode->PlayUISound(InteractSuccessSound, 1.f);

		}

		AfterInteraction();

	}
}

void AHappyInteractableSituation::AfterInteraction()
{
	Super::AfterInteraction();

	switch (situationType)
	{
	case EInteractSituationType::EIST_NormalDoor:
	case EInteractSituationType::EIST_BigDoor:
	case EInteractSituationType::EIST_GraveYardDoor:
	case EInteractSituationType::EIST_MainCatheralDoor:
	case EInteractSituationType::EIST_MainCatheralDoor2:

	case EInteractSituationType::EIST_CrossOverSecondDoor:
	case EInteractSituationType::EIST_CrossOverFirstDoor:
	case EInteractSituationType::EIST_GraveyardLock:
	case EInteractSituationType::EIST_CathedralLock:
	case EInteractSituationType::EIST_GardenLock :

		if (AdditiveStaticMesh1)
		{
			AdditiveStaticMesh1->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		}

		if (AdditiveStaticMesh2)
		{
			AdditiveStaticMesh2->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		}

		break;

	case EInteractSituationType::EIST_OneWayDoor:
		if (AdditiveStaticMesh1)
		{
			AdditiveStaticMesh1->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		}

		if (AdditiveStaticMesh2)
		{
			AdditiveStaticMesh2->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		}

		break;


	case EInteractSituationType::EIST_RedTrigger:
	case EInteractSituationType::EIST_BlueTrigger:
	case EInteractSituationType::EIST_GreenTrigger:
	case EInteractSituationType::EIST_YellowTrigger:

		// somethignt that can show tirgger is closed
		StaticMeshComponent->SetVisibility(false);
		break;

	case EInteractSituationType::EIST_RedTriggerDoor:
	case EInteractSituationType::EIST_BlueTriggerDoor:
	case EInteractSituationType::EIST_GreenTriggerDoor:
	case EInteractSituationType::EIST_YellowTriggerDoor:

		if (AdditiveStaticMesh1)
		{
			AdditiveStaticMesh1->SetStaticMesh(nullptr);
		}

		if (AdditiveStaticMesh2)
		{
			AdditiveStaticMesh2->SetStaticMesh(nullptr);
		}
		break;

	}
}

void AHappyInteractableSituation::CheckBroadCastItemIsMe(EInteractSituationType _situationType)
{
	if (_situationType != situationType) return;
	if (!bInteractWithUI) return;

	bAlreadyInteract = true;

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->SetAleradyInteract(GetName());
		PeaceFulHazardGameMode->PlayUISound(InteractSuccessSound, 1.f);
	}

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
		}else if(situationType == EInteractSituationType::EIST_SaveBox)
		{
			playerController->TriggerMenu_Save(true);
		}
		else if (situationType == EInteractSituationType::EIST_Note)
		{
			playerController->ShowInformationUI(true, NoteText);
			if (NotereadToDoUpdate != EPlayerToDo::EPTD_None)
			{
				if (PeaceFulHazardGameMode)
				{
					PeaceFulHazardGameMode->ToDoUpdate(NotereadToDoUpdate);
				}
			}
		}
		else if (situationType == EInteractSituationType::EIST_RedTrigger ||
			situationType == EInteractSituationType::EIST_BlueTrigger ||
			situationType == EInteractSituationType::EIST_GreenTrigger ||
			situationType == EInteractSituationType::EIST_YellowTrigger)
		{
			if (PeaceFulHazardGameMode)
			{
				PeaceFulHazardGameMode->SetAleradyInteract(GetName());
				PeaceFulHazardGameMode->TriggerDoorEvent.Broadcast(situationType);
			}

			AfterInteraction();
		}
		else if (situationType == EInteractSituationType::EIST_RedTriggerDoor ||
			situationType == EInteractSituationType::EIST_GreenTriggerDoor ||
			situationType == EInteractSituationType::EIST_YellowTriggerDoor)
		{
			if (PeaceFulHazardGameMode)
			{
				FString string = FString("Door is Locked. You should trigger someting to open this door");
				PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(true, string);
			}
		}
		else if (situationType == EInteractSituationType::EIST_BlueTriggerDoor)
		{
			if (PeaceFulHazardGameMode)
			{
				FString string = FString("Door is Locked. You should trigger someting to open this door");

				if (PeaceFulHazardGameMode->currentMapType == EWarpTarget::EWT_MainCathedral)
				{
					string = FString("To Take the key, you should trigger the blue points in this floor");
				}

				PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(true, string);
			}
		}
		else if (situationType == EInteractSituationType::EIST_OneWayDoor)
		{
			if (character && OneWayAllow && OneWayNotAllow)
			{
				float DistanceToAllow = FVector::Dist2D(character->GetActorLocation(), OneWayAllow->GetComponentLocation());
				float DistanceToNotAllow = FVector::Dist2D(character->GetActorLocation(), OneWayNotAllow->GetComponentLocation());

				if (DistanceToAllow > DistanceToNotAllow)
				{
					if (PeaceFulHazardGameMode)
					{
						FString string = FString("Doors Should be Opened in the Opposite Direction");
						PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(true, string);
					}
				}
				else
				{
					if (PeaceFulHazardGameMode)
					{
						PeaceFulHazardGameMode->SetAleradyInteract(GetName());
						PeaceFulHazardGameMode->PlayUISound(InteractSuccessSound, 1.f);

					}

					AfterInteraction();

				}
			}
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

	OneWayAllow = CreateDefaultSubobject<UBoxComponent>(TEXT("OneWayAllow"));
	OneWayAllow->SetupAttachment(StaticMeshComponent);

	OneWayNotAllow = CreateDefaultSubobject<UBoxComponent>(TEXT("OneWayNotAllow"));
	OneWayNotAllow->SetupAttachment(StaticMeshComponent);


	
}
