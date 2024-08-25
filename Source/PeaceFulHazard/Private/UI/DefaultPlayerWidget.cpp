// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DefaultPlayerWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

void UDefaultPlayerWidget::UpdateBulletUI(int32 currentBullet, int32 maxBullet, int32 leftBullet, EItemType itemType, int32 anotherBullet)
{
    FText CurrentBulletTextValue = FText::AsNumber(currentBullet);
    FText MaxBulletTextValue = FText::AsNumber(maxBullet);
    FText LeftBulletTextValue = FText::AsNumber(leftBullet);
    FText anotherBulletTextValue = FText::AsNumber(anotherBullet);

    CurrentBulletText->SetText(CurrentBulletTextValue);
    MaxBulletText->SetText(MaxBulletTextValue);
    LeftBulletText->SetText(LeftBulletTextValue);
    LeftAnotherBulletText->SetText(anotherBulletTextValue);

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

    if (anotherBullet == 0)
    {
        LeftAnotherBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))); // Red
    }
    else
    {
        LeftAnotherBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f))); // White
    }


    if (leftBullet == 0)
    {
        LeftBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))); // Red
    }
    else
    {
        LeftBulletText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f))); // White
    }

    if (itemType == EItemType::EIT_Bullet_Big)
    {
        PistolImage->SetColorAndOpacity(FLinearColor(1.0, 0.373266, 0.53436));
        PistolBulletImage->SetColorAndOpacity(FLinearColor(1.0, 0.208265, 0.218212));
        PistolAnotherBulletImage->SetColorAndOpacity(FLinearColor(0.489158, 0.666189, 1));

    }
    else 
    {
        PistolImage->SetColorAndOpacity(FLinearColor::White);
        PistolBulletImage->SetColorAndOpacity(FLinearColor(0.489158, 0.666189, 1));
        PistolAnotherBulletImage->SetColorAndOpacity(FLinearColor(1.0, 0.208265, 0.218212));
    }

    


}
