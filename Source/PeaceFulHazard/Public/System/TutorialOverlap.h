// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "TutorialOverlap.generated.h"

class UBoxComponent;
class APeaceFulHazardGameMode;

UCLASS()
class PEACEFULHAZARD_API ATutorialOverlap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialOverlap();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TutorialOverlapBox;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ETutorialType tutorialType;

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);



public:	

};
