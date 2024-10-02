// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenu_LoadWidget.h"
#include "MainMenu/MainMenuGameMode.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

#include "Components/CanvasPanelSlot.h"

#include "System/PeacFulGameInstance.h"
#include "System/PeacFulSaveGame.h"

#include "Kismet/GameplayStatics.h"

void UMainMenu_LoadWidget::BackUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Default, true);
	}
}

void UMainMenu_LoadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenuGameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PeacFulGameInstance = Cast<UPeacFulGameInstance>(GetGameInstance());

	if (MainMenuGameMode)
	{
		MainMenuGameMode->SaveFinishedEvent.AddDynamic(this, &ThisClass::UpdateAllUI);
		MainMenuGameMode->LanguageChangeEvent.AddDynamic(this, &ThisClass::UpdateAllUI);

	}

	SettingButton->OnClicked.AddDynamic(this, &ThisClass::OnSettingButtonClicked);
	NewGaemButton->OnClicked.AddDynamic(this, &ThisClass::OnNewGameButtonClicked);

	SettingButton->OnHovered.AddDynamic(this, &ThisClass::OnSettingButtonHovered);
	NewGaemButton->OnHovered.AddDynamic(this, &ThisClass::OnNewGameButtonHoverd);

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

}

void UMainMenu_LoadWidget::OnSettingButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Setting, true);
		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);

	}
}

void UMainMenu_LoadWidget::OnNewGameButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_NewGame, true);
		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);
	}
}

void UMainMenu_LoadWidget::OnSettingButtonHovered()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);

	}
}

void UMainMenu_LoadWidget::OnNewGameButtonHoverd()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);

	}
}

void UMainMenu_LoadWidget::SetDynamicChangeLanguage(UTextBlock* textBlock, const FText& Englishtext, const FText& Koreantext)
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

void UMainMenu_LoadWidget::UpdateAllUI()
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

void UMainMenu_LoadWidget::CheckLanguage()
{
	if (MainMenuGameMode == nullptr) return;

	SetLangaugeText(MainMenuGameMode->GetCurrentLanguage());

}


int32 UMainMenu_LoadWidget::GetButtonIndex(UButton* button, bool bSaveButtons)
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

void UMainMenu_LoadWidget::ChangeNowHoveringButton(UButton* button, bool bSaveButtons)
{
	HoveringSaveButton = button;

	UpdateAllUI();
}

void UMainMenu_LoadWidget::OnSaveButtonClicked()
{
	if (HoveringSaveButton == nullptr) return;
	if (MainMenuGameMode == nullptr) return;

	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonClickSound, 1.f);

		CheckCallBackHoveringSaveButton = HoveringSaveButton;

		if (CheckCallBackHoveringSaveButton)
		{
			FString ButtonName = CheckCallBackHoveringSaveButton->GetName();
			FString EnglishMessage = FString::Printf(TEXT("Do you want to load from slot \"%s\"?"), *ButtonName);

			FString KoreanMessage =  FString::Printf(TEXT("\"%s\" 슬롯을 불러오겠습니까?"), *ButtonName);

			MainMenuGameMode->CheckOneMoreMenuEvent.Broadcast(true, FText::FromString(EnglishMessage), FText::FromString(KoreanMessage));
			MainMenuGameMode->CheckOneMoreSuccessMenuEvent.AddDynamic(this, &ThisClass::OnceSaveButtonClickedSuccess);
		}


	}
}

void UMainMenu_LoadWidget::OnceSaveButtonClickedSuccess()
{
	if (CheckCallBackHoveringSaveButton == nullptr) return;
	if (MainMenuGameMode == nullptr) return;

	MainMenuGameMode->LoadDataFromSlot(CheckCallBackHoveringSaveButton->GetName(), false);

	MainMenuGameMode->CheckOneMoreSuccessMenuEvent.RemoveDynamic(this, &ThisClass::OnceSaveButtonClickedSuccess);

}

void UMainMenu_LoadWidget::OnSaveButtonHovered()
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

	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}

	ChangeNowHoveringButton(FindButton, true);

}

void UMainMenu_LoadWidget::OnSaveButtonUnhovered()
{
	ChangeNowHoveringButton(nullptr, true);

}



void UMainMenu_LoadWidget::OnContinueButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);

		FString EnglishMessage = FString::Printf(TEXT("Do you want to continue last game? "));

		FString KoreanMessage = FString::Printf(TEXT("가장 최근 게임을 이어서 하시겠습니까?"));

		MainMenuGameMode->CheckOneMoreMenuEvent.Broadcast(true, FText::FromString(EnglishMessage), FText::FromString(KoreanMessage));
		MainMenuGameMode->CheckOneMoreSuccessMenuEvent.AddDynamic(this, &ThisClass::OnceContinueButtonClickedSuccess);


	}
}

void UMainMenu_LoadWidget::OnceContinueButtonClickedSuccess()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->LoadDataFromContinue();
		MainMenuGameMode->CheckOneMoreSuccessMenuEvent.RemoveDynamic(this, &ThisClass::OnceContinueButtonClickedSuccess);

	}

}

void UMainMenu_LoadWidget::OnContinueButtonHoverd()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
}



void UMainMenu_LoadWidget::InitArrays()
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
