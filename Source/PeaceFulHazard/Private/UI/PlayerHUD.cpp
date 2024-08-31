// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/AimCrossHairWidget.h"
#include "UI/DefaultPlayerWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemBoxWidget.h"
#include "UI/NoticePanelWidget.h"
#include "UI/SaveWidget.h"

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

void APlayerHUD::SetMainMenuDisplay(bool bVisible, bool bSavePanelSave)
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
        SaveWidget->bSaveMode = bSavePanelSave;
        SaveWidget->UpdateAllUI();
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

    if (SaveWidget)
    {
        SaveWidget->BackUIInputTrigger();
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
