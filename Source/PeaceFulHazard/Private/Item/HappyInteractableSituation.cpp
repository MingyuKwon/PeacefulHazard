// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyInteractableSituation.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "Controller/HappyPlayerController.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Components/BoxComponent.h"

void AHappyInteractableSituation::FInalBattleCinematicShow(bool flag)
{
	if (PeaceFulHazardGameMode == nullptr) return;

	if (PeaceFulHazardGameMode->GetPlayerToDo() == EPlayerToDo::EPTD_Survive)
	{
		Super::AfterInteraction();

		if (flag)
		{
			switch (situationType)
			{
			case EInteractSituationType::EIST_SaveBox:
			case EInteractSituationType::EIST_ItemBox:

				Destroy();
				break;

			}
		}
		else
		{
			switch (situationType)
			{
			case EInteractSituationType::EIST_NormalDoor:
			case EInteractSituationType::EIST_BigDoor:
				if (AdditiveStaticMesh1 && DisableMaterial)
				{
					AdditiveStaticMesh1->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
					AdditiveStaticMesh1->SetMaterial(1, DisableMaterial);
				}

				if (AdditiveStaticMesh2 && DisableMaterial)
				{
					AdditiveStaticMesh2->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
					AdditiveStaticMesh2->SetMaterial(1, DisableMaterial);
				}
				break;

			case EInteractSituationType::EIST_GraveYardDoor:
			case EInteractSituationType::EIST_MainCatheralDoor:
			case EInteractSituationType::EIST_MainCatheralDoor2:

				if (AdditiveStaticMesh1 && DisableMaterial)
				{
					AdditiveStaticMesh1->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
					AdditiveStaticMesh1->SetMaterial(1, DisableMaterial);
				}

				if (AdditiveStaticMesh2 && DisableMaterial)
				{
					AdditiveStaticMesh2->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
					AdditiveStaticMesh2->SetMaterial(1, DisableMaterial);
				}

				break;
			}
		}
	}

}

void AHappyInteractableSituation::CheckToShow()
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

void AHappyInteractableSituation::BeginPlay()
{
	Super::BeginPlay();

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->InteractSituationEvent.AddDynamic(this, &ThisClass::CheckBroadCastItemIsMe);

		if (situationType == EInteractSituationType::EIST_FinalWarp)
		{
			SetActorVisibility(false);
			PeaceFulHazardGameMode->DynamicSpawnStartEvent.AddDynamic(this, &ThisClass::CheckToShow);

			return;
		}

		PeaceFulHazardGameMode->CinematicPlayEvent.AddDynamic(this, &ThisClass::FInalBattleCinematicShow);

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
			FString string = PeaceFulHazardGameMode->GetCurrentLanguage() == ELanguage::ED_English ? FString("Some Door placed in this map opened!") : FString(TEXT("이 구역 어딘가 있는 어떤 문이 열린 것 같다! "));
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
			FString string = PeaceFulHazardGameMode->GetCurrentLanguage() == ELanguage::ED_English ? FString("Some Door placed in this map opened!") : FString(TEXT("이 구역 어딘가 있는 어떤 문이 열린 것 같다! "));

			if (PeaceFulHazardGameMode->currentMapType == EWarpTarget::EWT_MainCathedral)
			{
				string = PeaceFulHazardGameMode->GetCurrentLanguage() == ELanguage::ED_English ? FString("Now you can get the key!") : FString(TEXT("이제 키를 가져 갈 수 있다!"));
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

		PeaceFulHazardGameMode->UpdateDefaultUIEvent.Broadcast();

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
			playerController->ShowInformationUI(true, PeaceFulHazardGameMode->GetCurrentLanguage() == ELanguage::ED_English ? NoteText : NoteTextKorean);

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
				FString string = PeaceFulHazardGameMode->GetCurrentLanguage() == ELanguage::ED_English ? FString("Door is Locked. \nYou should trigger someting to open this door") : FString(TEXT("문이 닫혀 있다. \n반대쪽 에서 열어야 할 것 같다 "));
				PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(true, string);
			}
		}
		else if (situationType == EInteractSituationType::EIST_BlueTriggerDoor)
		{
			if (PeaceFulHazardGameMode)
			{
				FString string = PeaceFulHazardGameMode->GetCurrentLanguage() == ELanguage::ED_English ? FString("Door is Locked. \nYou should trigger someting to open this door") : FString(TEXT("문이 닫혀 있다. \n뭔가 작동 시켜야 문이 열릴 것 같다 "));

				if (PeaceFulHazardGameMode->currentMapType == EWarpTarget::EWT_MainCathedral)
				{
					string = PeaceFulHazardGameMode->GetCurrentLanguage() == ELanguage::ED_English ? FString("To Take the key, you should trigger the blue points in this floor") : FString(TEXT("키를 가져가기 위해선, 이 층에 있는 푸른 트리거를 작동 시켜야 한다"));
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
						FString string = PeaceFulHazardGameMode->GetCurrentLanguage() == ELanguage::ED_English ? FString("Door is Locked. \nYou should trigger someting to open this door") : FString(TEXT("문이 닫혀 있다. \n반대쪽 에서 열어야 할 것 같다 "));
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
		}else if (situationType == EInteractSituationType::EIST_FinalWarp)
		{
			if (PeaceFulHazardGameMode)
			{
				UE_LOG(LogTemp, Display, TEXT("Game Clear"));
				PeaceFulHazardGameMode->GameClearEvent.Broadcast();
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
