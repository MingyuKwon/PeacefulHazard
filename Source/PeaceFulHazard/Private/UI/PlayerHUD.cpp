// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/AimCrossHairWidget.h"
#include "UI/DefaultPlayerWidget.h"
#include "UI/InventoryWidget.h"

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

void APlayerHUD::SetTabDisplay(bool bVisible)
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
    }
}

void APlayerHUD::SetBulletDisplay(int32 currentBullet, int32 maxBullet, int32 leftBullet)
{
    if (DefaultPlayerWidget != nullptr)
    {
        DefaultPlayerWidget->SetBulletUI(currentBullet, maxBullet, leftBullet);

        beforeCurrentBulle = -1;
        beforeMaxBullet = -1;
        beforeLeftBullet = -1;

    }
    else
    {
        beforeCurrentBulle = currentBullet;
        beforeMaxBullet = maxBullet;
        beforeLeftBullet = leftBullet;
    }
}

void APlayerHUD::BackUIInputTrigger()
{
    if (InventoryWidget)
    {
        InventoryWidget->BackUIInputTrigger();
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
                SetBulletDisplay(beforeCurrentBulle, beforeMaxBullet, beforeLeftBullet);
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
        }
    }
}