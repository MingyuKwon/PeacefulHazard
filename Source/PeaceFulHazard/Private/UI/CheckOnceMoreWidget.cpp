// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CheckOnceMoreWidget.h"
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
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "SlateCore.h"
#include "MainMenu/MainMenuGameMode.h"

void UCheckOnceMoreWidget::BackUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	SetOneMoreDisplay(false);
}

void UCheckOnceMoreWidget::OkUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	SetOneMoreDisplay(false);

	if (PeaceFulHazardGameMode == nullptr && MainMenuGameMode == nullptr) return;

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->CheckOneMoreSuccessGameEvent.Broadcast();
	}
	else if (MainMenuGameMode)
	{
		MainMenuGameMode->CheckOneMoreSuccessMenuEvent.Broadcast();

	}
}

void UCheckOnceMoreWidget::SetOneMoreDisplay(bool bVisible, const FText EnglishText, const FText KoreanText)
{
	if (!bVisible)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (PeaceFulHazardGameMode == nullptr && MainMenuGameMode == nullptr) return;

	CheckLanguage();

	ELanguage language = ELanguage::EL_None;

	if (PeaceFulHazardGameMode)
	{
		language = PeaceFulHazardGameMode->GetCurrentLanguage();
	}
	else if (MainMenuGameMode)
	{
		language = MainMenuGameMode->GetCurrentLanguage();
	}

	if (language == ELanguage::EL_None) return;

	SetVisibility(ESlateVisibility::Visible);

	if (language == ELanguage::ED_English)
	{
		CheckOnceText->SetText(EnglishText);
	}
	else if (language == ELanguage::ED_Korean)
	{
		CheckOnceText->SetText(KoreanText);

	}
}

void UCheckOnceMoreWidget::NativeConstruct()
{
	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainMenuGameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	PeacFulGameInstance = Cast<UPeacFulGameInstance>(GetGameInstance());

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->LanguageChangeEvent.AddDynamic(this, &ThisClass::CheckLanguage);
	}
	else if (MainMenuGameMode)
	{
		MainMenuGameMode->LanguageChangeEvent.AddDynamic(this, &ThisClass::CheckLanguage);
	}

}

void UCheckOnceMoreWidget::CheckLanguage()
{
	if (PeaceFulHazardGameMode == nullptr && MainMenuGameMode == nullptr) return;

	if (PeaceFulHazardGameMode)
	{
		SetLangaugeText(PeaceFulHazardGameMode->GetCurrentLanguage());
	}
	else if (MainMenuGameMode)
	{
		SetLangaugeText(MainMenuGameMode->GetCurrentLanguage());

	}

}
