// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOverWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include "System/PeacFulGameInstance.h"
#include "System/PeacFulSaveGame.h"
#include "GameMode/PeaceFulHazardGameMode.h"

#include "Kismet/GameplayStatics.h"


void UGameOverWidget::BackUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (SaveCanvas->GetVisibility() == ESlateVisibility::Hidden) return;

	SaveCanvas->SetVisibility(ESlateVisibility::Hidden);
}

void UGameOverWidget::SetGameEndVisibility(bool bVisible, bool bGameClear)
{
	if (!bVisible)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (bGameClear)
	{
		GameClearCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		GameClearCanvas->SetVisibility(ESlateVisibility::Hidden);

	}
	

}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PeacFulGameInstance = Cast<UPeacFulGameInstance>(GetGameInstance());

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->SaveFinishedEvent.AddDynamic(this, &ThisClass::UpdateAllUI);
		PeaceFulHazardGameMode->LanguageChangeEvent.AddDynamic(this, &ThisClass::UpdateAllUI);

	}

	InitArrays();

	UpdateAllUI();

	for (UButton* Button : SaveButtons)
	{
		if (Button)
		{
			Button->OnClicked.AddDynamic(this, &ThisClass::OnSaveButtonClicked);
			Button->OnHovered.AddDynamic(this, &ThisClass::OnSaveButtonHovered);
			Button->OnUnhovered.AddDynamic(this, &ThisClass::OnSaveButtonUnhovered);
		}
	}

	ContinueButton->OnClicked.AddDynamic(this, &ThisClass::OnContinueButtonClicked);
	ContinueButton->OnHovered.AddDynamic(this, &ThisClass::OnContinueButtonHoverd);

	LoadButton->OnClicked.AddDynamic(this, &ThisClass::OnLoadButtonClicked);
	LoadButton->OnHovered.AddDynamic(this, &ThisClass::OnLoadButtonHoverd);

	GoTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnGoTitleButtonClicked);
	GoTitleButton->OnHovered.AddDynamic(this, &ThisClass::OnGoTitleButtonHoverd);

	GoTitleButton_Clear->OnClicked.AddDynamic(this, &ThisClass::OnGoTitleButtonClicked);
	GoTitleButton_Clear->OnHovered.AddDynamic(this, &ThisClass::OnGoTitleButtonHoverd);

	SaveCanvas->SetVisibility(ESlateVisibility::Hidden);
	GameClearCanvas->SetVisibility(ESlateVisibility::Hidden);

}



void UGameOverWidget::SetDynamicChangeLanguage(UTextBlock* textBlock, const FText& Englishtext, const FText& Koreantext)
{
	if (PeaceFulHazardGameMode == nullptr) return;

	switch (PeaceFulHazardGameMode->GetCurrentLanguage())
	{
	case ELanguage::ED_English:
		textBlock->SetText(Englishtext);
		break;

	case ELanguage::ED_Korean:
		textBlock->SetText(Koreantext);
		break;

	}
}

void UGameOverWidget::UpdateAllUI()
{
	CheckLanguage();

	ContinueButton->SetIsEnabled(false);

	for (int32 i = 0; i < SaveButtons.Num(); i++)
	{
		if (SaveButtons[i])
		{
			SaveButtons[i]->SetIsEnabled(true);

			FString SlotName = SaveButtons[i]->GetName();

			if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
			{

				UPeacFulSaveGame* LoadedGame = Cast<UPeacFulSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

				if (PeacFulGameInstance)
				{
					FString temp = PeacFulGameInstance->GetToDoMapByLanguage()[PeacFulGameInstance->currentToDos[LoadedGame->saveTodoIndex]];
					ToDoTexts[i]->SetText(FText::FromString(temp));

					TimeTexts[i]->SetText(FText::FromString(LoadedGame->SaveTime.ToString(TEXT("%m/%d  |  %H:%M"))));

					SaveSpotTexts[i]->SetText(FText::FromString(PeacFulGameInstance->GetMapNameByLanguage()[LoadedGame->saveMapName]));

					switch (LoadedGame->SavegameDifficulty)
					{
					case EDifficulty::ED_Easy:
						SetDynamicChangeLanguage(DifficultyTexts[i], FText::FromString("Easy"), FText::FromString(TEXT("쉬움")));
						DifficultyTexts[i]->SetColorAndOpacity(FLinearColor::Green);
						break;

					case EDifficulty::ED_Normal:
						SetDynamicChangeLanguage(DifficultyTexts[i], FText::FromString("Normal"), FText::FromString(TEXT("보통")));
						DifficultyTexts[i]->SetColorAndOpacity(FLinearColor::Black);

						break;

					case EDifficulty::ED_Hard:
						SetDynamicChangeLanguage(DifficultyTexts[i], FText::FromString("Hard"), FText::FromString(TEXT("어려움")));
						DifficultyTexts[i]->SetColorAndOpacity(FLinearColor::Red);

						break;

					default:
						DifficultyTexts[i]->SetText(FText::FromString(""));

					}


					SaveSlotBackgrounds[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

					ContinueButton->SetIsEnabled(true);

				}

			}
			else
			{
				ToDoTexts[i]->SetText(FText::FromString(FString("")));
				SaveSlotBackgrounds[i]->SetVisibility(ESlateVisibility::Hidden);
				TimeTexts[i]->SetText(FText::FromString(FString("")));
				DifficultyTexts[i]->SetText(FText::FromString(""));

				SaveSpotTexts[i]->SetText(FText::FromString(FString("")));

				SaveButtons[i]->SetIsEnabled(false);
			}
		}
	}
}

void UGameOverWidget::CheckLanguage()
{
	if (PeaceFulHazardGameMode == nullptr) return;

	SetLangaugeText(PeaceFulHazardGameMode->GetCurrentLanguage());

}


int32 UGameOverWidget::GetButtonIndex(UButton* button, bool bSaveButtons)
{
	int32 index = 0;

	TArray<UButton*> tempArray = SaveButtons;

	for (UButton* Button : tempArray)
	{
		if (Button == button)
		{
			break;
		}

		index++;
	}

	return index < 8 ? index : -1;
}

void UGameOverWidget::ChangeNowHoveringButton(UButton* button, bool bSaveButtons)
{
	HoveringSaveButton = button;

	UpdateAllUI();
}

void UGameOverWidget::OnSaveButtonClicked()
{
	if (HoveringSaveButton == nullptr) return;
	if (PeaceFulHazardGameMode == nullptr) return;

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
		PeaceFulHazardGameMode->LoadDataFromSlot(HoveringSaveButton->GetName(), false);
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 0.7f);

	}
}


void UGameOverWidget::OnSaveButtonHovered()
{
	UButton* FindButton = nullptr;

	for (UButton* Button : SaveButtons)
	{
		if (Button->IsHovered())
		{
			FindButton = Button;
			break;
		}
	}

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}

	ChangeNowHoveringButton(FindButton, true);

}

void UGameOverWidget::OnSaveButtonUnhovered()
{
	ChangeNowHoveringButton(nullptr, true);

}

void UGameOverWidget::OnContinueButtonClicked()
{
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->ShowLoadingEvent.Broadcast(true);
		PeaceFulHazardGameMode->LoadDataFromContinue();
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 0.7f);

	}
}

void UGameOverWidget::OnContinueButtonHoverd()
{
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}

void UGameOverWidget::OnLoadButtonClicked()
{
	SaveCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}

void UGameOverWidget::OnLoadButtonHoverd()
{
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}

void UGameOverWidget::OnGoTitleButtonClicked()
{
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
		PeaceFulHazardGameMode->ShowLoadingEvent.Broadcast(true);
		PeaceFulHazardGameMode->MoveToMainMenu();

	}

}

void UGameOverWidget::OnGoTitleButtonHoverd()
{
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}

void UGameOverWidget::InitArrays()
{
	SaveButtons.Add(SaveButton1);
	SaveButtons.Add(SaveButton2);
	SaveButtons.Add(SaveButton3);
	SaveButtons.Add(SaveButton4);
	SaveButtons.Add(SaveButton5);
	SaveButtons.Add(SaveButton6);
	SaveButtons.Add(SaveButton7);
	SaveButtons.Add(SaveButton8);

	ToDoTexts.Add(ToDoText1);
	ToDoTexts.Add(ToDoText2);
	ToDoTexts.Add(ToDoText3);
	ToDoTexts.Add(ToDoText4);
	ToDoTexts.Add(ToDoText5);
	ToDoTexts.Add(ToDoText6);
	ToDoTexts.Add(ToDoText7);
	ToDoTexts.Add(ToDoText8);

	SaveSlotBackgrounds.Add(SaveSlotBackground1);
	SaveSlotBackgrounds.Add(SaveSlotBackground2);
	SaveSlotBackgrounds.Add(SaveSlotBackground3);
	SaveSlotBackgrounds.Add(SaveSlotBackground4);
	SaveSlotBackgrounds.Add(SaveSlotBackground5);
	SaveSlotBackgrounds.Add(SaveSlotBackground6);
	SaveSlotBackgrounds.Add(SaveSlotBackground7);
	SaveSlotBackgrounds.Add(SaveSlotBackground8);

	TimeTexts.Add(TimeText1);
	TimeTexts.Add(TimeText2);
	TimeTexts.Add(TimeText3);
	TimeTexts.Add(TimeText4);
	TimeTexts.Add(TimeText5);
	TimeTexts.Add(TimeText6);
	TimeTexts.Add(TimeText7);
	TimeTexts.Add(TimeText8);

	SaveSpotTexts.Add(SaveSpotText1);
	SaveSpotTexts.Add(SaveSpotText2);
	SaveSpotTexts.Add(SaveSpotText3);
	SaveSpotTexts.Add(SaveSpotText4);
	SaveSpotTexts.Add(SaveSpotText5);
	SaveSpotTexts.Add(SaveSpotText6);
	SaveSpotTexts.Add(SaveSpotText7);
	SaveSpotTexts.Add(SaveSpotText8);

	DifficultyTexts.Add(DifficultyText1);
	DifficultyTexts.Add(DifficultyText2);
	DifficultyTexts.Add(DifficultyText3);
	DifficultyTexts.Add(DifficultyText4);
	DifficultyTexts.Add(DifficultyText5);
	DifficultyTexts.Add(DifficultyText6);
	DifficultyTexts.Add(DifficultyText7);
	DifficultyTexts.Add(DifficultyText8);

}

