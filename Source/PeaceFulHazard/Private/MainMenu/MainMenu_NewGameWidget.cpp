// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenu_NewGameWidget.h"
#include "MainMenu/MainMenuGameMode.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "System/PeacFulGameInstance.h"

void UMainMenu_NewGameWidget::BackUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Default, true);
	}
}

void UMainMenu_NewGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenuGameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (MainMenuGameMode)
	{
		MainMenuGameMode->LanguageChangeEvent.AddDynamic(this, &ThisClass::CheckLanguage);

	}

	LoadButton->OnClicked.AddDynamic(this, &ThisClass::OnLoadButtonClicked);
	SettingButton->OnClicked.AddDynamic(this, &ThisClass::OnSettingButtonClicked);

	LoadButton->OnHovered.AddDynamic(this, &ThisClass::OnLoadButtonHovered);
	SettingButton->OnHovered.AddDynamic(this, &ThisClass::OnSettingButtonHoverd);


	
	GameStartButton->OnClicked.AddDynamic(this, &ThisClass::OnGameStartButtonlicked);
	GameStartButton->OnHovered.AddDynamic(this, &ThisClass::OnGameStartButtonHoverd);

	

	EasyButton->OnClicked.AddDynamic(this, &ThisClass::OnEasyButtonClicked);
	NormalButton->OnClicked.AddDynamic(this, &ThisClass::OnNormalButtonClicked);
	HardButton->OnClicked.AddDynamic(this, &ThisClass::OnHardButtonClicked);

	EasyButton->OnHovered.AddDynamic(this, &ThisClass::OnEasyButtonHover);
	NormalButton->OnHovered.AddDynamic(this, &ThisClass::OnNormalButtonHover);
	HardButton->OnHovered.AddDynamic(this, &ThisClass::OnHardButtonHover);

	CheckLanguage();

	OnNormalButtonClicked();
	OnNormalButtonHover();
}

void UMainMenu_NewGameWidget::CheckLanguage()
{
	if (MainMenuGameMode == nullptr) return;

	SetLangaugeText(MainMenuGameMode->GetCurrentLanguage());

	if (currentSelectDifficulty == EDifficulty::ED_Easy)
	{
		OnEasyButtonClicked();
		OnEasyButtonHover();
	}
	else if (currentSelectDifficulty == EDifficulty::ED_Normal)
	{
		OnNormalButtonClicked();
		OnNormalButtonHover();
	}
	else if (currentSelectDifficulty == EDifficulty::ED_Hard)
	{
		OnHardButtonClicked();
		OnHardButtonHover();
	}

}

void UMainMenu_NewGameWidget::SetDynamicChangeLanguage(UTextBlock* textBlock, const FText& Englishtext, const FText& Koreantext)
{
	if (MainMenuGameMode == nullptr) return;

	switch (MainMenuGameMode->GetCurrentLanguage())
	{
	case ELanguage::ED_English:
		textBlock->SetText(Englishtext);
		break;

	case ELanguage::ED_Korean:
		textBlock->SetText(Koreantext);
		break;

	}
}


void UMainMenu_NewGameWidget::OnGameStartButtonlicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_GameStart, true);

		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, [this]() {
			MainMenuGameMode->LoadingShowEvent.Broadcast(1.f);
			MainMenuGameMode->LoadDataFromSlot(FString(""), true);

			}, 1.f, false);
	}

}

void UMainMenu_NewGameWidget::OnGameStartButtonHoverd()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}

void UMainMenu_NewGameWidget::OnEasyButtonClicked()
{
	if (PeacFulGameInstance)
	{
		PeacFulGameInstance->gameDifficulty = EDifficulty::ED_Easy;

		SetDynamicChangeLanguage(DifficultyText, FText::FromString(FString("Easy")), FText::FromString(FString(TEXT("쉬움"))));
		DifficultyText->SetColorAndOpacity(FLinearColor::Green);

		currentSelectDifficulty = EDifficulty::ED_Easy;

		if (MainMenuGameMode)
		{
			MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);
		}

	}
}

void UMainMenu_NewGameWidget::OnNormalButtonClicked()
{
	if (PeacFulGameInstance)
	{
		PeacFulGameInstance->gameDifficulty = EDifficulty::ED_Normal;

		SetDynamicChangeLanguage(DifficultyText, FText::FromString(FString("Normal")), FText::FromString(FString(TEXT("보통"))));
		DifficultyText->SetColorAndOpacity(FLinearColor::Black);

		currentSelectDifficulty = EDifficulty::ED_Normal;

		if (MainMenuGameMode)
		{
			MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);
		}
	}
}

void UMainMenu_NewGameWidget::OnHardButtonClicked()
{
	if (PeacFulGameInstance)
	{
		PeacFulGameInstance->gameDifficulty = EDifficulty::ED_Hard;

		SetDynamicChangeLanguage(DifficultyText, FText::FromString(FString("Hard")), FText::FromString(FString(TEXT("어려움"))));
		DifficultyText->SetColorAndOpacity(FLinearColor::Red);

		currentSelectDifficulty = EDifficulty::ED_Hard;

		if (MainMenuGameMode)
		{
			MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);
		}
	}
}

void UMainMenu_NewGameWidget::OnEasyButtonHover()
{
	EasyShot->SetVisibility(ESlateVisibility::Visible);
	NormalShot->SetVisibility(ESlateVisibility::Hidden);
	HardShot->SetVisibility(ESlateVisibility::Hidden);

	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}

	SetDynamicChangeLanguage(DifficultyExaplainText, FText::FromString(FString("This is the easiest difficulty level.\n\n- Enemies are weaker.\n- The number of items you can obtain increases.\n- You have more inventory space.\n\nRecommended for those playing TPS games for the first time.")), FText::FromString(FString(TEXT("가장 쉬운 난이도 입니다. \n\n- 적들이 더 약해집니다 \n- 얻을 수 있는 아이템의 수가 늘어납니다\n- 인벤토리가 더 여유롭습니다.\n\nTPS류 게임을 처음 해보는 분들께 추천합니다"))));
}

void UMainMenu_NewGameWidget::OnNormalButtonHover()
{
	EasyShot->SetVisibility(ESlateVisibility::Hidden);
	NormalShot->SetVisibility(ESlateVisibility::Visible);
	HardShot->SetVisibility(ESlateVisibility::Hidden);

	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}

	SetDynamicChangeLanguage(DifficultyExaplainText, FText::FromString(FString("This is the standard difficulty level.\n\nRecommended for those with experience in TPS games \nor who have played games like Resident Evil.")), FText::FromString(FString(TEXT("일반적인 난이도 입니다\n\nTPS류 게임에 경험이 있거나, \n바이오하자드 류 게임을 플레이 해보신 분들께 추천드립니다."))));

}

void UMainMenu_NewGameWidget::OnHardButtonHover()
{
	EasyShot->SetVisibility(ESlateVisibility::Hidden);
	NormalShot->SetVisibility(ESlateVisibility::Hidden);
	HardShot->SetVisibility(ESlateVisibility::Visible);

	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}

	SetDynamicChangeLanguage(DifficultyExaplainText, FText::FromString(FString("This is the hardest difficulty level.\n\n- Enemies become stronger.\n- The number of items you can obtain decreases.\n- Inventory space is more limited.\n\nRecommended for players who are familiar with the game.")), FText::FromString(FString(TEXT("가장 어려운 난이도 입니다. \n\n- 적들이 더 강해집니다 \n- 얻을 수 있는 아이템의 수가 적어집니다\n- 인벤토리가 더 빡빡해집니다\n\n게임 플레이에 익숙해진 분들에게 추천합니다"))));

}

void UMainMenu_NewGameWidget::OnLoadButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Load, true);
		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);

	}

}

void UMainMenu_NewGameWidget::OnSettingButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Setting, true);
		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);
	}
}

void UMainMenu_NewGameWidget::OnLoadButtonHovered()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}

void UMainMenu_NewGameWidget::OnSettingButtonHoverd()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}
