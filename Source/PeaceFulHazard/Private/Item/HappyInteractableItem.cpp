// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyInteractableItem.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "UI/CanInteractWidget.h"

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

    if (BoxComponent)
    {
        BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
        BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);

    }

    if (WidgetComponent)
    {
        CanInteractWidget = Cast<UCanInteractWidget>(WidgetComponent->GetUserWidgetObject());

        if (CanInteractWidget)
        {
            CanInteractWidget->SetInteractEnable(false);
            CanInteractWidget->SetActionEnable(false);
        }
    }

	
}

// Called every frame
void AHappyInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHappyInteractableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        APeaceFulHazardCharacter* OverlappingCharacter = Cast<APeaceFulHazardCharacter>(OtherActor);
        if (OverlappingCharacter)
        {
            if (OtherComp == OverlappingCharacter->InteractBox)
            {
                UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with InteractBox %s"), *OtherActor->GetName());

                if (CanInteractWidget)
                {
                    CanInteractWidget->SetInteractEnable(true);
                }
            }

            if (OtherComp == OverlappingCharacter->actiontBox)
            {
                UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with actiontBox %s"), *OtherActor->GetName());

                if (CanInteractWidget)
                {
                    CanInteractWidget->SetActionEnable(true);
                }
            }

        }
    }
}

void AHappyInteractableItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        APeaceFulHazardCharacter* OverlappingCharacter = Cast<APeaceFulHazardCharacter>(OtherActor);
        if (OverlappingCharacter)
        {
            if (OtherComp == OverlappingCharacter->InteractBox)
            {
                UE_LOG(LogTemp, Warning, TEXT("Overlap End with InteractBox %s"), *OtherActor->GetName());

                if (CanInteractWidget)
                {
                    CanInteractWidget->SetInteractEnable(false);
                }
            }

            if (OtherComp == OverlappingCharacter->actiontBox)
            {
                UE_LOG(LogTemp, Warning, TEXT("Overlap End with actiontBox %s"), *OtherActor->GetName());

                if (CanInteractWidget)
                {
                    CanInteractWidget->SetActionEnable(false);
                }
            }
        }
    }
}
