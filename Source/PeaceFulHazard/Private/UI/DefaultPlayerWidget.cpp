// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DefaultPlayerWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "System/PeacFulSaveGame.h"
#include "System/PeacFulGameInstance.h"

#include "Components/ProgressBar.h"

void UDefaultPlayerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(this));

    if (PeaceFulHazardGameMode)
    {
        if (PeaceFulHazardGameMode->GetPlayerToDo() == EPlayerToDo::EPTD_Survive && PeaceFulHazardGameMode->currentMapType == EWarpTarget::EWT_MainHub)
        {
            PeaceFulHazardGameMode->FinalTimeShowEvent.AddDynamic(this, &ThisClass::SetTImeTextFunction);
            FinalTimeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
            FinalTimeBackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

        }
        else
        {
            FinalTimeText->SetVisibility(ESlateVisibility::Hidden);
            FinalTimeBackGround->SetVisibility(ESlateVisibility::Hidden);

        }

    }
}

void UDefaultPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

}

void UDefaultPlayerWidget::SetTImeTextFunction(int32 timeSeconds)
{
    int32 minutes = timeSeconds / 60;
    int32 seconds = timeSeconds % 60;

    FString TimeText = FString::Printf(TEXT("%d : %02d"), minutes, seconds);

    if (timeSeconds <= 0)
    {
        TimeText = FString("Warp!");
    }

    FinalTimeText->SetText(FText::FromString(TimeText));


    FLinearColor Color;

    if (timeSeconds > 260)
    {
        Color = FLinearColor(1.0f, 0.5f, 0.5f);

    }
    else if (timeSeconds > 220)
    {
        Color = FLinearColor(1.0f, 0.4f, 0.4f);

    }
    else if (timeSeconds > 170)
    {
        Color = FLinearColor(1.0f, 0.3f, 0.3f);

    }
    else if (timeSeconds > 80)
    {
        Color = FLinearColor(1.0f, 0.2f, 0.2f);

    }
    else
    {
        Color = FLinearColor(1.0f, 0.0f, 0.0f);

    }

    FinalTimeText->SetColorAndOpacity(FSlateColor(Color));

}



void UDefaultPlayerWidget::UpdateBulletUI(int32 currentBullet, int32 maxBullet, int32 leftBullet, EItemType itemType, int32 anotherBullet)
{
    FText CurrentBulletTextValue = FText::AsNumber(currentBullet);
    FText MaxBulletTextValue = FText::AsNumber(maxBullet);
    FText LeftBulletTextValue = FText::AsNumber(leftBullet);
    FText anotherBulletTextValue = FText::AsNumber(anotherBullet);

    CurrentBulletText->SetText(CurrentBulletTextValue);
    MaxBulletText->SetText(MaxBulletTextValue);

    if (currentBullet == maxBullet)
    {
        CurrentBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f))); // Green
    }
    else if (currentBullet == 0)
    {
        CurrentBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))); // Red
    }
    else
    {
        CurrentBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f))); // White
    }


    if (itemType == EItemType::EIT_Bullet_Big)
    {
        PistolImage->SetColorAndOpacity(FLinearColor(1.0, 0.373266, 0.53436));

        NowBackGround->SetVisibility(ESlateVisibility::Hidden);
        AnotherBackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

        LeftAnotherBulletText->SetText(LeftBulletTextValue);
        LeftBulletText->SetText(anotherBulletTextValue);

    }
    else 
    {
        PistolImage->SetColorAndOpacity(FLinearColor::White);

        AnotherBackGround->SetVisibility(ESlateVisibility::Hidden);
        NowBackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

        LeftBulletText->SetText(LeftBulletTextValue);
        LeftAnotherBulletText->SetText(anotherBulletTextValue);

    }


    if (anotherBullet == 0)
    {
        if (itemType == EItemType::EIT_Bullet_Big)
        {
            LeftBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))); // Red
        }
        else
        {
            LeftAnotherBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))); // Red
        }
    }
    else
    {
        if (itemType == EItemType::EIT_Bullet_Big)
        {
            LeftBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f))); // Red
        }
        else
        {
            LeftAnotherBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f))); // Red
        }
    }


    if (leftBullet == 0)
    {
        if (itemType == EItemType::EIT_Bullet_Big)
        {
            LeftAnotherBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))); // Red
        }
        else
        {
            LeftBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))); // Red
        }
    }
    else
    {
        if (itemType == EItemType::EIT_Bullet_Big)
        {
            LeftAnotherBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f))); // Red
        }
        else
        {
            LeftBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f))); // White
        }
    }


}

void UDefaultPlayerWidget::UpdateTodoUI()
{
    if (PeaceFulHazardGameMode == nullptr) return;
    if (PeacFulGameInstance == nullptr) return;

    FString temp;
    PeacFulGameInstance->GetCurrentToDo(temp);

    TodoText->SetText(FText::FromString(temp));

}

void UDefaultPlayerWidget::ShowLoadingUI(bool bVisible)
{
    UE_LOG(LogTemp, Warning, TEXT("ShowLoadingUI %s"), bVisible ? *FString("True") : *FString("False"));

    if (bVisible)
    {
        LoadingCanvas->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        LoadingCanvas->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UDefaultPlayerWidget::UpdateHealthUI(float health, int32 currentForce)
{
    if (PeacFulGameInstance == nullptr) return;

    showHealth = health;

    Healthbar->SetPercent(showHealth);

    int32 gap = currentForce - PeacFulGameInstance->currentEnemyForce;

    myForceText->SetText(FText::AsNumber(currentForce));
    ForceGapText->SetText(FText::AsNumber(gap));

    if (gap < 0)
    {
        ForceGapText->SetColorAndOpacity(FLinearColor::Red);

    }
    else if (gap > 0)
    {
        ForceGapText->SetColorAndOpacity(FLinearColor::Green);

    }
    else
    {
        ForceGapText->SetColorAndOpacity(FLinearColor::Gray);

    }

}

