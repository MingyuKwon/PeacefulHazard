// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyInteractableSituation.h"

void AHappyInteractableSituation::InteractWithPlayer(APeaceFulHazardCharacter* character)
{

}

AHappyInteractableSituation::AHappyInteractableSituation()
{
	AdditiveStaticMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AdditiveStaticMesh1"));
	AdditiveStaticMesh1->SetupAttachment(StaticMeshComponent);

	AdditiveStaticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AdditiveStaticMesh2"));
	AdditiveStaticMesh2->SetupAttachment(StaticMeshComponent);

}
