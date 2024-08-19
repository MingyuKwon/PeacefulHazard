// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyInteractableItem.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AHappyInteractableItem::AHappyInteractableItem()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(BoxComponent);

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(BoxComponent);

}

// Called when the game starts or when spawned
void AHappyInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHappyInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

