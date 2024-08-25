// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/AimCrossHairWidget.h"
#include "UI/DefaultPlayerWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemBoxWidget.h"
#include "UI/NoticePanelWidget.h"

void APlayerHUD::DrawHUD()
{
    Super::DrawHUD();
}

void APlayerHUD::SetAimDisplay(bool bVisible)
{
    if (!bVisible)
    {
        if (HUDAimWidget)
        {
            HUDAimWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        return;
    }

    if (HUDAimWidget)
    {
        HUDAimWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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

    }
}

void APlayerHUD::UpdateBulletDisplay(int32 currentBullet, int32 maxBullet, int32 leftBullet, EItemType itemType)
{
    if (DefaultPlayerWidget != nullptr)
    {
        DefaultPlayerWidget->UpdateBulletUI(currentBullet, maxBullet, leftBullet, itemType);

        beforeCurrentBulle = -1;
        beforeMaxBullet = -1;
        beforeLeftBullet = -1;
        beforeBulletType = EItemType::EIT_Bullet_Noraml;

    }
    else
    {
        beforeCurrentBulle = currentBullet;
        beforeMaxBullet = maxBullet;
        beforeLeftBullet = leftBullet;
        beforeBulletType = itemType;
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
    
    if (NoticePanelWidget)
    {
        NoticePanelWidget->BackUIInputTrigger();
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

    if (AimCrossHairWidgetClass != nullptr)
    {
        HUDAimWidget = CreateWidget<UAimCrossHairWidget>(GetWorld(), AimCrossHairWidgetClass);
        if (HUDAimWidget != nullptr)
        {
            HUDAimWidget->AddToViewport();
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
                UpdateBulletDisplay(beforeCurrentBulle, beforeMaxBullet, beforeLeftBullet, beforeBulletType);
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
