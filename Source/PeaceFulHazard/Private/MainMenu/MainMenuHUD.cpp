// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuHUD.h"
#include "MainMenu/MainMenu_NewGameWidget.h"
#include "MainMenu/MainMenu_SettingWidget.h"
#include "MainMenu/MainMenu_LoadWidget.h"
#include "MainMenu/MainMenu_DefaultWidget.h"

#include "MainMenu/MainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuHUD::BackUIInputTrigger()
{
    if (MainMenu_NewGameWidget)
    {
        MainMenu_NewGameWidget->BackUIInputTrigger();
    }

    if (MainMenu_SettingWidget)
    {
        MainMenu_SettingWidget->BackUIInputTrigger();
    }

    if (MainMenu_LoadWidget)
    {
        MainMenu_LoadWidget->BackUIInputTrigger();
    }

    if (MainMenu_DefaultWidget)
    {
        MainMenu_DefaultWidget->BackUIInputTrigger();
    }
}

void AMainMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    MainMenuGameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));
    if (MainMenuGameMode)
    {
        MainMenuGameMode->MenuModeChangeEvent.AddDynamic(this, &ThisClass::ChangeMenuMode);
    }

    if (MainMenu_NewGameWidgetClass != nullptr)
    {
        MainMenu_NewGameWidget = CreateWidget<UMainMenu_NewGameWidget>(GetWorld(), MainMenu_NewGameWidgetClass);
        if (MainMenu_NewGameWidget != nullptr)
        {
            MainMenu_NewGameWidget->AddToViewport();
            MainMenu_NewGameWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (MainMenu_SettingWidgetClass != nullptr)
    {
        MainMenu_SettingWidget = CreateWidget<UMainMenu_SettingWidget>(GetWorld(), MainMenu_SettingWidgetClass);
        if (MainMenu_SettingWidget != nullptr)
        {
            MainMenu_SettingWidget->AddToViewport();
            MainMenu_SettingWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (MainMenu_LoadWidgetClass != nullptr)
    {
        MainMenu_LoadWidget = CreateWidget<UMainMenu_LoadWidget>(GetWorld(), MainMenu_LoadWidgetClass);
        if (MainMenu_LoadWidget != nullptr)
        {
            MainMenu_LoadWidget->AddToViewport();
            MainMenu_LoadWidget->SetVisibility(ESlateVisibility::Hidden);

        }
    }

    if (MainMenu_DefaultWidgetClass != nullptr)
    {
        MainMenu_DefaultWidget = CreateWidget<UMainMenu_DefaultWidget>(GetWorld(), MainMenu_DefaultWidgetClass);
        if (MainMenu_DefaultWidget != nullptr)
        {
            MainMenu_DefaultWidget->AddToViewport();
        }

    }


}

void AMainMenuHUD::ChangeMenuMode(EMainMenuType menuType, bool bChangeStart)
{
    if (bChangeStart)
    {
        ChangeShowMenuUI(EMainMenuType::EMT_None);
    }
    else
    {
        ChangeShowMenuUI(menuType);
    }
}

void AMainMenuHUD::ChangeShowMenuUI(EMainMenuType menuType)
{
    if (menuType == EMainMenuType::EMT_None || menuType == EMainMenuType::EMT_GameStart)
    {
        MainMenu_NewGameWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_SettingWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_LoadWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_DefaultWidget->SetVisibility(ESlateVisibility::Hidden);

    }
    else if (menuType == EMainMenuType::EMT_Load)
    {
        MainMenu_NewGameWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_SettingWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_LoadWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        MainMenu_DefaultWidget->SetVisibility(ESlateVisibility::Hidden);

    }
    else if (menuType == EMainMenuType::EMT_Setting)
    {
        MainMenu_NewGameWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_SettingWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        MainMenu_LoadWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_DefaultWidget->SetVisibility(ESlateVisibility::Hidden);

    }
    else if (menuType == EMainMenuType::EMT_NewGame)
    {
        MainMenu_NewGameWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        MainMenu_SettingWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_LoadWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_DefaultWidget->SetVisibility(ESlateVisibility::Hidden);

    }
    else if (menuType == EMainMenuType::EMT_Default)
    {
        MainMenu_NewGameWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_SettingWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_LoadWidget->SetVisibility(ESlateVisibility::Hidden);
        MainMenu_DefaultWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    }


}

