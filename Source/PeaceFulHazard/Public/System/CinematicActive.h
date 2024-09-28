// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "CinematicActive.generated.h"

class ULevelSequence;
class APeaceFulHazardGameMode;

UCLASS()
class PEACEFULHAZARD_API ACinematicActive : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACinematicActive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Cinematics")
	TSoftObjectPtr<ULevelSequence> LevelSequenceToPlay;

	UPROPERTY(EditAnywhere, Category = "Cinematics")
	EPlayerToDo sequenceShowToDo = EPlayerToDo::EPTD_None;

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	UFUNCTION()
	void MapStartCallBack();

	UFUNCTION()
	void OnSequenceFinished();

	

	void PlaySequence();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
