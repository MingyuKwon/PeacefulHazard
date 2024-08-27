// Fill out your copyright notice in the Description page of Project Settings.


#include "System/TutorialOverlap.h"
#include "Components/BoxComponent.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "System/PeacFulGameInstance.h"

// Sets default values
ATutorialOverlap::ATutorialOverlap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TutorialOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TutorialOverlapBox"));
	TutorialOverlapBox->SetupAttachment(RootComponent);

	
}

void ATutorialOverlap::BeginPlay()
{
	Super::BeginPlay();

	if (TutorialOverlapBox)
	{
		TutorialOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	}

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(this));

	PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void ATutorialOverlap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        APeaceFulHazardCharacter* OverlappingCharacter = Cast<APeaceFulHazardCharacter>(OtherActor);

        if (OverlappingCharacter)
        {
			if (PeaceFulHazardGameMode && PeacFulGameInstance)
			{
				if (!PeacFulGameInstance->checkIsTutorialAlready(tutorialType))
				{
					PeaceFulHazardGameMode->TutorialEvent.Broadcast(tutorialType);
				}
				
			}
        }
    }
}




