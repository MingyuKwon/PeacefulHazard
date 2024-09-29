// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenu_SettingWidget.h"
#include "MainMenu/MainMenuGameMode.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu_SettingWidget::BackUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Default, true);
	}
}

void UMainMenu_SettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenuGameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	LoadButton->OnClicked.AddDynamic(this, &ThisClass::OnLoadButtonClicked);
	NewGaemButton->OnClicked.AddDynamic(this, &ThisClass::OnNewGameButtonClicked);

}

void UMainMenu_SettingWidget::OnLoadButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Load, true);
	}
}

void UMainMenu_SettingWidget::OnNewGameButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_NewGame, true);

	}
}
