// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuLoadingWidget.h"
#include "MainMenu/MainMenuGameMode.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuLoadingWidget::SetLoadingOpacity(float Opacity)
{
    FLinearColor CurrentColor = LoadingBackGround->GetBrushColor();
    CurrentColor.A = Opacity; 

    LoadingBackGround->SetBrushColor(CurrentColor);

    if (Opacity <= 0.4f)
    {
        LoadingText->SetVisibility(ESlateVisibility::Hidden);
        LoadingBackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    else
    {
        LoadingText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        LoadingBackGround->SetVisibility(ESlateVisibility::Visible);

    }
}

void UMainMenuLoadingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    MainMenuGameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (MainMenuGameMode)
    {
        MainMenuGameMode->LoadingShowEvent.AddDynamic(this, &ThisClass::SetLoadingOpacity);
    }

    SetLoadingOpacity(1.f);
}
