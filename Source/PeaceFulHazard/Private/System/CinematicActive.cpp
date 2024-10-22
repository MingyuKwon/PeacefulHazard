// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CinematicActive.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h" 

// Sets default values
ACinematicActive::ACinematicActive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACinematicActive::BeginPlay()
{
	Super::BeginPlay();

    if (sequenceShowToDo == EPlayerToDo::EPTD_None)
    {
        Destroy();
        return;
    }
	
    PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(this));
    if (PeaceFulHazardGameMode)
    {
        if (PeaceFulHazardGameMode->GetPlayerToDo() >= sequenceShowToDo)
        {
            PeaceFulHazardGameMode->MapStartEvent.AddDynamic(this, &ThisClass::MapStartCallBack);
        }
        else
        {
            Destroy();
        }


    }

}

void ACinematicActive::MapStartCallBack()
{
    if (PeaceFulHazardGameMode)
    {
        if (!PeaceFulHazardGameMode->CheckAleradyInteract(GetName()))
        {
            PlaySequence();

        }
        else
        {
            Destroy();

        }
    }
}

void ACinematicActive::OnSequenceFinished()
{
    if (PeaceFulHazardGameMode) PeaceFulHazardGameMode->CinematicPlayEvent.Broadcast(false);

}

void ACinematicActive::PlaySequence()
{
    if (LevelSequenceToPlay)
    {
        ALevelSequenceActor* SequenceActor;
        ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(), LevelSequenceToPlay.Get(), FMovieSceneSequencePlaybackSettings(), SequenceActor);

        if (SequencePlayer)
        {
            SequencePlayer->OnFinished.AddDynamic(this, &ThisClass::OnSequenceFinished);
            if (PeaceFulHazardGameMode) PeaceFulHazardGameMode->CinematicPlayEvent.Broadcast(true);
            
            SequencePlayer->Play();

            PeaceFulHazardGameMode->SetAleradyInteract(GetName());

        }
    }
}

// Called every frame
void ACinematicActive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

