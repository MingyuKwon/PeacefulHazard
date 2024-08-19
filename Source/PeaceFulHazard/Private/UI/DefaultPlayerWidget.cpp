// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DefaultPlayerWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

void UDefaultPlayerWidget::SetBulletUI(int32 currentBullet, int32 maxBullet, int32 leftBullet)
{
    FText CurrentBulletTextValue = FText::AsNumber(currentBullet);
    FText MaxBulletTextValue = FText::AsNumber(maxBullet);

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

}

