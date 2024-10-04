// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HappyInteractableItem.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/PeaceFulHazardCharacter.h"
#include "UI/CanInteractWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PeaceFulHazardGameMode.h"

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
            CanInteractWidget->SetInteractEnable(bCanInteractable, false, DoorInteractkeyImage, GetKeyName());
            CanInteractWidget->SetActionEnable(bCanInteractable, false);
        }
    }

    PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(this));
    if (PeaceFulHazardGameMode)
    {
        PeaceFulHazardGameMode->NowAimingEvent.AddDynamic(this, &ThisClass::AImVisibleUI);
        PeaceFulHazardGameMode->CloseAllUIEvent.AddDynamic(this, &ThisClass::UICloseAllInteractWithItemOver);
        PeaceFulHazardGameMode->MapStartEvent.AddDynamic(this, &ThisClass::MapStartCallBack);
    }

}

void AHappyInteractableItem::SetActorVisibility(bool flag)
{
    SetAdditiveVisibility(flag);

    if (flag)
    {
        BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        StaticMeshComponent->SetVisibility(true);

    }
    else
    {
        BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        StaticMeshComponent->SetVisibility(false);
    }
}

FString AHappyInteractableItem::GetKeyName()
{
    if (PeaceFulHazardGameMode == nullptr) return FString("");


    switch (PeaceFulHazardGameMode->GetCurrentLanguage())
    {
    case ELanguage::ED_English:
        return DoorInteractkeyNameEnglish;
        break;

    case ELanguage::ED_Korean:
        return DoorInteractkeyNameKorean;
        break;

    }

    return FString("");
}

void AHappyInteractableItem::AImVisibleUI(bool flag)
{
    bCanInteractable = !flag;

    if (flag)
    {
        if (CanInteractWidget)
        {
            CanInteractWidget->SetInteractEnable(bCanInteractable, false, DoorInteractkeyImage, GetKeyName());
            CanInteractWidget->SetActionEnable(bCanInteractable, false);
        }
    }
    else
    {
        if (CanInteractWidget)
        {
            CanInteractWidget->SetInteractEnable(bCanInteractable, bIntearctable, DoorInteractkeyImage, GetKeyName());
            CanInteractWidget->SetActionEnable(bCanInteractable, bActionable);
        }
    }
}

// Called every frame
void AHappyInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHappyInteractableItem::SetbActionable(bool flag)
{
    bActionable = flag;
    if (CanInteractWidget)
    {
        CanInteractWidget->SetActionEnable(bCanInteractable, bActionable);
    }
}

void AHappyInteractableItem::InteractWithPlayer(APeaceFulHazardCharacter* character)
{
    DestroyItem();
}

void AHappyInteractableItem::DestroyItem()
{
    if (OverlappingCharacter)
    {
        OverlappingCharacter->RemoveCurrentActionableItem(this);
    }
    Destroy();

}

void AHappyInteractableItem::MapStartCallBack()
{
    if (PeaceFulHazardGameMode->CheckAleradyInteract(GetName()))
    {
        AfterInteraction();
    }
}

void AHappyInteractableItem::AfterInteraction()
{
    WidgetComponent->SetVisibility(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHappyInteractableItem::UICloseAllInteractWithItemOver()
{
    bInteractWithUI = false;
}

void AHappyInteractableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        OverlappingCharacter = Cast<APeaceFulHazardCharacter>(OtherActor);

        if (OverlappingCharacter)
        {
            if (OtherComp == OverlappingCharacter->InteractBox)
            {

                bIntearctable = true;
                if (CanInteractWidget)
                {
                    UE_LOG(LogTemp, Display, TEXT("bCanInteractable : %s, bIntearctable : %s"), bCanInteractable ? *FString("True") : *FString("False"), bIntearctable ? *FString("True") : *FString("False"));


                    CanInteractWidget->SetInteractEnable(bCanInteractable, bIntearctable, DoorInteractkeyImage, GetKeyName());
                }
            }

            if (OtherComp == OverlappingCharacter->actiontBox)
            {
                OverlappingCharacter->AddCurrentActionableItem(this);
            }

        }
    }
}

void AHappyInteractableItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        OverlappingCharacter = Cast<APeaceFulHazardCharacter>(OtherActor);

        if (OverlappingCharacter)
        {
            if (OtherComp == OverlappingCharacter->InteractBox)
            {
                bIntearctable = false;

                if (CanInteractWidget)
                {
                    CanInteractWidget->SetInteractEnable(bCanInteractable, bIntearctable, DoorInteractkeyImage, GetKeyName());
                }
            }

            if (OtherComp == OverlappingCharacter->actiontBox)
            {
                OverlappingCharacter->RemoveCurrentActionableItem(this);
            }

        }
    }
}
