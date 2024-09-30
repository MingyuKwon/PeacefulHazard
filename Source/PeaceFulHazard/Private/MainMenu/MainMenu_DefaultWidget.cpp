// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenu_DefaultWidget.h"
#include "MainMenu/MainMenuGameMode.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu_DefaultWidget::BackUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

}

void UMainMenu_DefaultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenuGameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (MainMenuGameMode)
	{
		MainMenuGameMode->LanguageChangeEvent.AddDynamic(this, &ThisClass::CheckLanguage);
	}

	QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitButtonClicked);
	LoadButton->OnClicked.AddDynamic(this, &ThisClass::OnLoadButtonClicked);
	SettingButton->OnClicked.AddDynamic(this, &ThisClass::OnSettingButtonClicked);
	NewGaemButton->OnClicked.AddDynamic(this, &ThisClass::OnNewGameButtonClicked);

	QuitGameButton->OnHovered.AddDynamic(this, &ThisClass::OnQuitButtonHovered);
	LoadButton->OnHovered.AddDynamic(this, &ThisClass::OnLoadButtonHovered);
	SettingButton->OnHovered.AddDynamic(this, &ThisClass::OnSettingButtonHovered);
	NewGaemButton->OnHovered.AddDynamic(this, &ThisClass::OnNewGameButtonHovered);

	CheckLanguage();
}

void UMainMenu_DefaultWidget::CheckLanguage()
{
	if (MainMenuGameMode == nullptr) return;

	SetLangaugeText(MainMenuGameMode->GetCurrentLanguage());

}

void UMainMenu_DefaultWidget::OnQuitButtonClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, true);

}

void UMainMenu_DefaultWidget::OnLoadButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Load, true);
		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);

	}
}

void UMainMenu_DefaultWidget::OnSettingButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Setting, true);
		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);

	}
}

void UMainMenu_DefaultWidget::OnNewGameButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_NewGame, true);
		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);

	}
}

void UMainMenu_DefaultWidget::OnQuitButtonHovered()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}

void UMainMenu_DefaultWidget::OnLoadButtonHovered()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}

void UMainMenu_DefaultWidget::OnSettingButtonHovered()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}

void UMainMenu_DefaultWidget::OnNewGameButtonHovered()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}
