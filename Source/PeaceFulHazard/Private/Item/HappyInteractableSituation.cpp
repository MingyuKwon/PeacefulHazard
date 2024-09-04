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


		if (situationType == EInteractSituationType::EIST_GraveyardTriggerDoor ||
			situationType == EInteractSituationType::EIST_CathedralTriggerDoor)
		{
			PeaceFulHazardGameMode->TriggerDoorEvent.AddDynamic(this, &ThisClass::ListenTirggerOn);
		}
	}

}

void AHappyInteractableSituation::ListenTirggerOn(EInteractSituationType _situationType)
{
	if (situationType == EInteractSituationType::EIST_GraveyardTriggerDoor && _situationType == EInteractSituationType::EIST_GraveyardTrigger)
	{
		if (PeaceFulHazardGameMode)
		{
			FString string = FString("Some Door placed in this map opened!");
			PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(true, string);
			PeaceFulHazardGameMode->SetAleradyInteract(GetName());
		}

		AfterInteraction();

	}

	if (situationType == EInteractSituationType::EIST_CathedralTriggerDoor && _situationType == EInteractSituationType::EIST_CathedralTrigger)
	{
		if (PeaceFulHazardGameMode)
		{
			FString string = FString("Some Door placed in this map opened!");
			PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(true, string);
			PeaceFulHazardGameMode->SetAleradyInteract(GetName());
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


	case EInteractSituationType::EIST_GraveyardTrigger:
	case EInteractSituationType::EIST_CathedralTrigger:

		// somethignt that can show tirgger is closed
		StaticMeshComponent->SetVisibility(false);
		break;

	case EInteractSituationType::EIST_GraveyardTriggerDoor:
	case EInteractSituationType::EIST_CathedralTriggerDoor:

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
		}
		else if (situationType == EInteractSituationType::EIST_GraveyardTrigger ||
			situationType == EInteractSituationType::EIST_CathedralTrigger)
		{
			if (PeaceFulHazardGameMode)
			{
				PeaceFulHazardGameMode->SetAleradyInteract(GetName());
				PeaceFulHazardGameMode->TriggerDoorEvent.Broadcast(situationType);
			}

			AfterInteraction();
		}
		else if (situationType == EInteractSituationType::EIST_GraveyardTriggerDoor)
		{
			if (PeaceFulHazardGameMode)
			{
				FString string = FString("Door is Locked. You should trigger someting to open this door");
				PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(true, string);
			}
		}
		else if (situationType == EInteractSituationType::EIST_CathedralTriggerDoor)
		{
			if (PeaceFulHazardGameMode)
			{
				FString string = FString("To Take the key, you should trigger two points in this floor");
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
