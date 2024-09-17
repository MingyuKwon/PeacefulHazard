// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/AimCrossHairWidget.h"
#include "UI/DefaultPlayerWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemBoxWidget.h"
#include "UI/NoticePanelWidget.h"
#include "UI/SaveWidget.h"
#include "UI/InformationPanelWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PeaceFulHazardGameMode.h"

void APlayerHUD::DrawHUD()
{
    Super::DrawHUD();
}

void APlayerHUD::SetAimDisplay(bool bVisible)
{
    if (HUDAimWidget->GetVisibility() == ESlateVisibility::SelfHitTestInvisible && bVisible) return;
    if (HUDAimWidget->GetVisibility() == ESlateVisibility::Hidden && !bVisible) return;


    if (!bVisible)
    {
        if (HUDAimWidget)
        {
            HUDAimWidget->SetVisibility(ESlateVisibility::Hidden);

            if (PeaceFulHazardGameMode)
            {
                PeaceFulHazardGameMode->PlayUISound(AimUICloseSound, 1.f);
            }
        }
        return;
    }

    if (HUDAimWidget)
    {
        HUDAimWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

        if (PeaceFulHazardGameMode)
        {
            PeaceFulHazardGameMode->PlayUISound(AimUIShowSound, 1.f);
        }
        
    }

}

void APlayerHUD::SetAimWide(float value)
{
    if (HUDAimWidget)
    {
        HUDAimWidget->setAimWide(value);
    }
}

void APlayerHUD::SetInventoryDisplay(bool bVisible)
{
    if (!bVisible)
    {
        if (InventoryWidget)
        {
            InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
            InventoryWidget->CloseInventoryUIValue();

            
        }
        return;
    }

    if (InventoryWidget)
    {
        InventoryWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        InventoryWidget->showTabUI();

        if (PeaceFulHazardGameMode)
        {
            PeaceFulHazardGameMode->PlayUISound(InventoryUIShowSound, 1.f);
        }
    }
}

void APlayerHUD::SetItemBoxDisplay(bool bVisible)
{
    if (!bVisible)
    {
        if (ItemBoxWidget)
        {
            ItemBoxWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        return;
    }

    if (ItemBoxWidget)
    {
        ItemBoxWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        ItemBoxWidget->showItemBoxUI();

        if (PeaceFulHazardGameMode)
        {
            PeaceFulHazardGameMode->PlayUISound(BoxUIShowSound, 1.f);
        }
    }

}

void APlayerHUD::SetNoticeDisplay(bool bVisible)
{
    if (!bVisible)
    {
        if (NoticePanelWidget)
        {
            NoticePanelWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        return;
    }

    if (NoticePanelWidget)
    {
        NoticePanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

        if (PeaceFulHazardGameMode)
        {
            PeaceFulHazardGameMode->PlayUISound(NoticeUIShowSound, 1.f);
        }
    }

}

void APlayerHUD::SetInformationDisplay(bool bVisible)
{
    if (!bVisible)
    {
        if (InformationPanelWidget)
        {
            InformationPanelWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        return;
    }

    if (InformationPanelWidget)
    {
        InformationPanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

        if (PeaceFulHazardGameMode)
        {
            PeaceFulHazardGameMode->PlayUISound(NoticeUIShowSound, 1.f);
        }
    }
}

void APlayerHUD::SetMainMenuDisplay(bool bVisible, EMenuType menuType, bool bSavePanelSave)
{
    if (!bVisible)
    {
        if (SaveWidget)
        {
            SaveWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        return;
    }

    if (SaveWidget)
    {
        SaveWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

        SaveWidget->SetMainMenuDisplay(menuType, bSavePanelSave);
      
        if (PeaceFulHazardGameMode)
        {
            PeaceFulHazardGameMode->PlayUISound(MenuUIShowSound, 1.f);
        }
    }
}

void APlayerHUD::ShowLoadingUI(bool bVisible)
{
    if (DefaultPlayerWidget)
    {
        DefaultPlayerWidget->ShowLoadingUI(bVisible);
    }
    else
    {
        bBeforeLoadingShow = true;
    }
}

void APlayerHUD::SetGetItemDisplay(bool bVisible, EItemType itemType, int32 count)
{
    if (!bVisible)
    {
        if (InventoryWidget)
        {
            InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        return;
    }

    if (InventoryWidget)
    {
        InventoryWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        InventoryWidget->showItemGetUI(itemType, count);

        if (PeaceFulHazardGameMode)
        {
            PeaceFulHazardGameMode->PlayUISound(GetItemUIShowSound,  1.f);
        }

    }
}

void APlayerHUD::showSituationUI(bool bVisible, EInteractSituationType situationType)
{
    if (!bVisible)
    {
        if (InventoryWidget)
        {
            InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        return;
    }

    if (InventoryWidget)
    {
        InventoryWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        InventoryWidget->showSituationUI(situationType);
        if (PeaceFulHazardGameMode)
        {
            PeaceFulHazardGameMode->PlayUISound(SituationUIShowSound, 1.f);
        }
    }
}

void APlayerHUD::UpdateBulletDisplay(int32 currentBullet, int32 maxBullet, int32 leftBullet, EItemType itemType, int32 anotherBullet)
{
    if (DefaultPlayerWidget != nullptr)
    {
        DefaultPlayerWidget->UpdateBulletUI(currentBullet, maxBullet, leftBullet, itemType, anotherBullet);

        beforeCurrentBulle = -1;
        beforeMaxBullet = -1;
        beforeLeftBullet = -1;
        beforeBulletType = EItemType::EIT_Bullet_Noraml;
        beforeanotherBullet = -1;

    }
    else
    {
        beforeCurrentBulle = currentBullet;
        beforeMaxBullet = maxBullet;
        beforeLeftBullet = leftBullet;
        beforeBulletType = itemType;
        beforeanotherBullet = anotherBullet;

    }
}

void APlayerHUD::UpdateTodoUI()
{
    if (DefaultPlayerWidget != nullptr)
    {
        DefaultPlayerWidget->UpdateTodoUI();
    }
}

void APlayerHUD::UpdateHealthUI(float health, int32 currentForce)
{
    if (DefaultPlayerWidget != nullptr)
    {
        DefaultPlayerWidget->UpdateHealthUI(health, currentForce);
    }
}

void APlayerHUD::UpdateInventoryDisplay(FCharacterInventoty* inventory)
{
    if (InventoryWidget)
    {
        InventoryWidget->SetInventoryDisplay(inventory);
    }
    else
    {
        beforeInventory = inventory;
    }
}

void APlayerHUD::UpdateItemBoxDisplay(FCharacterInventoty* inventory, FCharacterItemBox* itemBox)
{
    if (ItemBoxWidget)
    {
        ItemBoxWidget->SetItemBoxDisplay(inventory, itemBox);
    }
    else
    {
        beforeInventory = inventory;
        beforeItemBox = itemBox;
    }
}

void APlayerHUD::UpdateNoticeDisplay(FString& noticeText)
{
    if (NoticePanelWidget)
    {
        NoticePanelWidget->UpdateNoticeDisplay(noticeText);
    }

}

void APlayerHUD::UpdateInformationDisplay(FString& noticeText)
{
    if (InformationPanelWidget)
    {
        InformationPanelWidget->UpdateNoticeDisplay(noticeText);
    }
}

void APlayerHUD::BackNoticeUIInputTrigger()
{
    if (NoticePanelWidget->GetVisibility() != ESlateVisibility::Hidden)
    {
        if (NoticePanelWidget)
        {
            NoticePanelWidget->BackUIInputTrigger();
        }
    }
    else if (InformationPanelWidget->GetVisibility() != ESlateVisibility::Hidden)
    {
        if (InformationPanelWidget)
        {
            InformationPanelWidget->BackUIInputTrigger();
        }
    }
    
    if (PeaceFulHazardGameMode)
    {
        PeaceFulHazardGameMode->PlayUISound(BackUISound, 0.5f);
    }
}

void APlayerHUD::BackMenuUIInputTrigger()
{
    if (SaveWidget)
    {
        SaveWidget->BackUIInputTrigger();

        if (PeaceFulHazardGameMode)
        {
            PeaceFulHazardGameMode->PlayUISound(BackUISound, 0.5f);
        }
    }

    
}

void APlayerHUD::BackUIInputTrigger()
{
    if (InventoryWidget)
    {
        InventoryWidget->BackUIInputTrigger();
    }

    if (ItemBoxWidget)
    {
        ItemBoxWidget->BackUIInputTrigger();
    }


    if (InformationPanelWidget)
    {
        InformationPanelWidget->BackUIInputTrigger();
    }

    if (PeaceFulHazardGameMode)
    {
        PeaceFulHazardGameMode->PlayUISound(BackUISound, 0.5f);
    }
}

void APlayerHUD::OkUIInputTrigger()
{
    if (InventoryWidget)
    {
        InventoryWidget->OkUIInputTrigger();
    }

    if (ItemBoxWidget)
    {
        ItemBoxWidget->OkUIInputTrigger();
    }

    if (NoticePanelWidget)
    {
        NoticePanelWidget->OkUIInputTrigger();
    }

    if (InformationPanelWidget)
    {
        InformationPanelWidget->OkUIInputTrigger();
    }

    if (SaveWidget)
    {
        SaveWidget->OkUIInputTrigger();
    }
}

bool APlayerHUD::GetCanCloseTab()
{
    if (InventoryWidget)
    {
        return !(InventoryWidget->InteractLock);
    }

    return false;
}

void APlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(this));

    if (AimCrossHairWidgetClass != nullptr)
    {
        HUDAimWidget = CreateWidget<UAimCrossHairWidget>(GetWorld(), AimCrossHairWidgetClass);
        if (HUDAimWidget != nullptr)
        {
            HUDAimWidget->AddToViewport();
            HUDAimWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (DefaultPlayerWidgetClass != nullptr)
    {
        DefaultPlayerWidget = CreateWidget<UDefaultPlayerWidget>(GetWorld(), DefaultPlayerWidgetClass);
        if (DefaultPlayerWidget != nullptr)
        {
            DefaultPlayerWidget->AddToViewport();

            if (beforeCurrentBulle >= 0)
            {
                UpdateBulletDisplay(beforeCurrentBulle, beforeMaxBullet, beforeLeftBullet, beforeBulletType, beforeanotherBullet);
            }

            if (bBeforeLoadingShow)
            {
                DefaultPlayerWidget->ShowLoadingUI(true);
                bBeforeLoadingShow = false;
            }
   

        }
    }

    if (InventoryWidgetClass != nullptr)
    {
        InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
        if (InventoryWidget != nullptr)
        {
            InventoryWidget->AddToViewport();
            InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

            if (beforeInventory != nullptr)
            {
                UpdateInventoryDisplay(beforeInventory);
            }
        }
    }

    if (ItemBoxWidgetClass != nullptr)
    {
        ItemBoxWidget = CreateWidget<UItemBoxWidget>(GetWorld(), ItemBoxWidgetClass);
        if (ItemBoxWidget != nullptr)
        {
            ItemBoxWidget->AddToViewport();
            ItemBoxWidget->SetVisibility(ESlateVisibility::Hidden);

            if (beforeInventory != nullptr && beforeItemBox != nullptr)
            {
                UpdateItemBoxDisplay(beforeInventory,beforeItemBox);
            }
        }
        
    }

    if (NoticePanelWidgetClass != nullptr)
    {
        NoticePanelWidget = CreateWidget<UNoticePanelWidget>(GetWorld(), NoticePanelWidgetClass);
        if (NoticePanelWidget != nullptr)
        {
            NoticePanelWidget->AddToViewport();
            NoticePanelWidget->SetVisibility(ESlateVisibility::Hidden);
        }

    }

    if (InformationPanelWidgetClass != nullptr)
    {
        InformationPanelWidget = CreateWidget<UInformationPanelWidget>(GetWorld(), InformationPanelWidgetClass);
        if (InformationPanelWidget != nullptr)
        {
            InformationPanelWidget->AddToViewport();
            InformationPanelWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (SaveWidgetClass != nullptr)
    {
        SaveWidget = CreateWidget<USaveWidget>(GetWorld(), SaveWidgetClass);
        if (SaveWidget != nullptr)
        {
            SaveWidget->AddToViewport();
            SaveWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    
    

    SetItemInformationToDisplay();
}

void APlayerHUD::SetItemInformationToDisplay()
{
    if (ItemInformationClass == nullptr) return;

    AItemInformation* ItemInformation = GetWorld()->SpawnActor<AItemInformation>(ItemInformationClass);
    
    if (InventoryWidget)
    {
        InventoryWidget->ItemInformation = ItemInformation;
    }

    if (ItemBoxWidget)
    {
        ItemBoxWidget->ItemInformation = ItemInformation;
    }
}
