// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SaveWidget.h"
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


void USaveWidget::BackUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->MenuUIChangeEvent.Broadcast(false);
	}
}

void USaveWidget::OkUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

}

int32 USaveWidget::GetButtonIndex(UButton* button, bool bSaveButtons)
{
	int32 index = 0;

	TArray<UButton*> tempArray = bSaveButtons ? SaveButtons : DeleteButtons;

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

void USaveWidget::ChangeNowHoveringButton(UButton* button, bool bSaveButtons)
{
	if (bSaveButtons)
	{
		HoveringSaveButton = button;
	}
	else
	{
		HoveringDeleteButton = button;
	}

	UpdateAllUI();
}

void USaveWidget::OnSaveButtonClicked()
{
	if (HoveringSaveButton == nullptr) return;
	if (PeaceFulHazardGameMode == nullptr) return;

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}

	if (bSaveMode)
	{
		PeaceFulHazardGameMode->SaveDataToSlot(HoveringSaveButton->GetName());
	}
	else
	{
		PeaceFulHazardGameMode->LoadDataFromSlot(HoveringSaveButton->GetName(), false);
	}
}

void USaveWidget::OnSaveButtonHovered()
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

void USaveWidget::OnSaveButtonUnhovered()
{
	ChangeNowHoveringButton(nullptr, true);
}

void USaveWidget::OnDeleteButtonClicked()
{
	if (!bSaveMode) return;
	if (HoveringDeleteButton == nullptr) return;

	int32 index = GetButtonIndex(HoveringDeleteButton, false);

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}

	if (bSaveMode)
	{
		PeaceFulHazardGameMode->DeleteDataFromSlot(SaveButtons[index]->GetName());
	}
}

void USaveWidget::OnDeleteButtonHovered()
{
	if (!bSaveMode) return;

	UButton* FindButton = nullptr;

	for (UButton* Button : DeleteButtons)
	{
		if (Button->IsHovered())
		{
			FindButton = Button;
			break;
		}
	}

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonHoverSound, 0.3f);
	}

	ChangeNowHoveringButton(FindButton, false);


}

void USaveWidget::OnDeleteButtonUnhovered()
{
	if (!bSaveMode) return;

	ChangeNowHoveringButton(nullptr, false);


}

void USaveWidget::OnNewButtonClicked()
{
	if (bSaveMode) return;

	if (PeaceFulHazardGameMode == nullptr) return;

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}

	PeaceFulHazardGameMode->LoadDataFromSlot(FString(""), true);

}

void USaveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->SaveFinishedEvent.AddDynamic(this, &ThisClass::UpdateAllUI);
	}

	InitArrays();


	if (SettingResetButton)
	{
		SettingResetButton->OnClicked.AddDynamic(this, &ThisClass::OnResetSettingValue);

	}

	for (UButton* Button : SaveButtons)
	{
		if (Button)
		{
			Button->OnClicked.AddDynamic(this, &ThisClass::OnSaveButtonClicked);
			Button->OnHovered.AddDynamic(this, &ThisClass::OnSaveButtonHovered);
			Button->OnUnhovered.AddDynamic(this, &ThisClass::OnSaveButtonUnhovered);
		}
	}

	for (UButton* Button : DeleteButtons)
	{
		if (Button)
		{
			Button->OnClicked.AddDynamic(this, &ThisClass::OnDeleteButtonClicked);
			Button->OnHovered.AddDynamic(this, &ThisClass::OnDeleteButtonHovered);
			Button->OnUnhovered.AddDynamic(this, &ThisClass::OnDeleteButtonUnhovered);
		}
	}

	if (NewSaveButton)
	{
		NewSaveButton->OnClicked.AddDynamic(this, &ThisClass::OnNewButtonClicked);
	}
	

	if (MapButton)
	{
		MapButton->OnClicked.AddDynamic(this, &ThisClass::OnMapButtonClicked);
	}
	if (SaveLoadButton)
	{
		SaveLoadButton->OnClicked.AddDynamic(this, &ThisClass::OnSaveLoadButtonClicked);
	}
	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnOptionButtonClicked);
	}
	if (TitleButton)
	{
		TitleButton->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
	}


	if (DefaultMouseSensibility)
	{
		DefaultMouseSensibility->OnValueChanged.AddDynamic(this, &ThisClass::OnDefaultMouseSensibilityValueChanged);
	}

	if (AimMouseSensibility)
	{
		AimMouseSensibility->OnValueChanged.AddDynamic(this, &ThisClass::OnAimMouseSensibilityValueChanged);
	}

	UpdateAllUI();
}

void USaveWidget::OnDefaultMouseSensibilityValueChanged(float Value)
{
	if (DefaultMouseSensibility && DefaultMouseSensibilityFill)
	{
		UCanvasPanelSlot* CanvasSlotFill = Cast<UCanvasPanelSlot>(DefaultMouseSensibilityFill->Slot);

		if (CanvasSlotFill)
		{
			FMargin NewOffsets = CanvasSlotFill->GetOffsets();
			NewOffsets.Right = maxDefaultMouseSensibilityFill * (1 - Value);
			CanvasSlotFill->SetOffsets(NewOffsets);
		}

		if (PeacFulGameInstance)
		{
			PeacFulGameInstance->MouseSensitivity = Value;
		}
	}

}

void USaveWidget::OnAimMouseSensibilityValueChanged(float Value)
{
	if (AimMouseSensibility && AimMouseSensibilityFill)
	{
		UCanvasPanelSlot* CanvasSlotFill = Cast<UCanvasPanelSlot>(AimMouseSensibilityFill->Slot);

		if (CanvasSlotFill)
		{
			FMargin NewOffsets = CanvasSlotFill->GetOffsets();
			NewOffsets.Right = maxAimMouseSensibilityFill * (1 - Value);

			CanvasSlotFill->SetOffsets(NewOffsets);

		}

		if (PeacFulGameInstance)
		{
			PeacFulGameInstance->MouseAimSensitivity = Value;
		}
	}
}

void USaveWidget::OnResetSettingValue()
{
	if (PeacFulGameInstance)
	{
		PeacFulGameInstance->ResetSetting();
	}

	UpdateAllUI();
}

void USaveWidget::OnMapButtonClicked()
{
	SetMainMenuDisplay(EMenuType::EMT_Map);

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}
}

void USaveWidget::OnSaveLoadButtonClicked()
{
	SetMainMenuDisplay(EMenuType::EMT_Save);

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}
}

void USaveWidget::OnOptionButtonClicked()
{
	SetMainMenuDisplay(EMenuType::EMT_Option);

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}
}

void USaveWidget::OnExitButtonClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, true);

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}
}



void USaveWidget::ShowCurrentLocation()
{
	if (PeaceFulHazardGameMode == nullptr) return;

	switch (PeaceFulHazardGameMode->GetDifficulty())
	{
	case EDifficulty::ED_Easy:
		MapDifficulty->SetText(FText::FromString("Easy"));
		MapDifficulty->SetColorAndOpacity(FLinearColor::Green);
		break;

	case EDifficulty::ED_Normal:
		MapDifficulty->SetText(FText::FromString("Normal"));
		MapDifficulty->SetColorAndOpacity(FLinearColor::Black);

		break;

	case EDifficulty::ED_Hard:
		MapDifficulty->SetText(FText::FromString("Hard"));
		MapDifficulty->SetColorAndOpacity(FLinearColor::Red);

		break;

	default:
		MapDifficulty->SetText(FText::FromString(""));

	}

	MainHubBorder->SetBrushColor(FLinearColor::White);

	GraveyardBorder->SetBrushColor(FLinearColor::White);

	CathedralBorder->SetBrushColor(FLinearColor::White);
	CathedralBorder2->SetBrushColor(FLinearColor::White);
	CathedralBorder3->SetBrushColor(FLinearColor::White);
	CathedralBorder4->SetBrushColor(FLinearColor::White);

	TutorialBorder->SetBrushColor(FLinearColor::White);
	TutorialBorder2->SetBrushColor(FLinearColor::White);
	TutorialBorder3->SetBrushColor(FLinearColor::White);
	TutorialBorder4->SetBrushColor(FLinearColor::White);
	TutorialBorder5->SetBrushColor(FLinearColor::White);

	CrossOverBorder->SetBrushColor(FLinearColor::White);
	CrossOverBorder2->SetBrushColor(FLinearColor::White);
	CrossOverBorder3->SetBrushColor(FLinearColor::White);
	CrossOverBorder4->SetBrushColor(FLinearColor::White);
	CrossOverBorder5->SetBrushColor(FLinearColor::White);

	LeftGardenBorder->SetBrushColor(FLinearColor::White);

	RightGardenBorder->SetBrushColor(FLinearColor::White);


	switch (PeaceFulHazardGameMode->currentMapType)
	{
	case EWarpTarget::EWT_MainHub :
		MainHubBorder->SetBrushColor(FLinearColor::Green);
		break;
	case EWarpTarget::EWT_GraveYard:
		GraveyardBorder->SetBrushColor(FLinearColor::Green);
		break;
	case EWarpTarget::EWT_MainCathedral:
		CathedralBorder->SetBrushColor(FLinearColor::Green);
		CathedralBorder2->SetBrushColor(FLinearColor::Green);
		CathedralBorder3->SetBrushColor(FLinearColor::Green);
		CathedralBorder4->SetBrushColor(FLinearColor::Green);

		break;
	case EWarpTarget::EWT_Tutorial:
		TutorialBorder->SetBrushColor(FLinearColor::Green);
		TutorialBorder2->SetBrushColor(FLinearColor::Green);
		TutorialBorder3->SetBrushColor(FLinearColor::Green);
		TutorialBorder4->SetBrushColor(FLinearColor::Green);
		TutorialBorder5->SetBrushColor(FLinearColor::Green);
		break;

	case EWarpTarget::EWT_CrossOver:
		CrossOverBorder->SetBrushColor(FLinearColor::Green);
		CrossOverBorder2->SetBrushColor(FLinearColor::Green);
		CrossOverBorder3->SetBrushColor(FLinearColor::Green);
		CrossOverBorder4->SetBrushColor(FLinearColor::Green);
		CrossOverBorder5->SetBrushColor(FLinearColor::Green);
		break;

	case EWarpTarget::EWT_LeftGarden:
		LeftGardenBorder->SetBrushColor(FLinearColor::Green);
		break;

	case EWarpTarget::EWT_RightGarden:
		RightGardenBorder->SetBrushColor(FLinearColor::Green);
		break;
	}


}

void USaveWidget::UpdateAllUI()
{
	if (PeaceFulHazardGameMode)
	{
		float defaultMouse;
		float aimMouse;
		PeaceFulHazardGameMode->GetSettingValue(defaultMouse, aimMouse);

		if (DefaultMouseSensibility)
		{
			DefaultMouseSensibility->SetValue(defaultMouse);
		}

		if (AimMouseSensibility)
		{
			AimMouseSensibility->SetValue(aimMouse);
		}
	}
	


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
					FString temp = PeacFulGameInstance->ToDoMap[PeacFulGameInstance->currentToDos[LoadedGame->saveTodoIndex]];
					ToDoTexts[i]->SetText(FText::FromString(temp));

					TimeTexts[i]->SetText(FText::FromString(LoadedGame->SaveTime.ToString(TEXT("%m/%d  |  %H:%M"))));

					SaveSpotTexts[i]->SetText(FText::FromString(PeacFulGameInstance->MapName[LoadedGame->saveMapName]));

					switch (LoadedGame->SavegameDifficulty)
					{
					case EDifficulty::ED_Easy :
						DifficultyTexts[i]->SetText(FText::FromString("Easy"));
						DifficultyTexts[i]->SetColorAndOpacity(FLinearColor::Green);
						break;

					case EDifficulty::ED_Normal:
						DifficultyTexts[i]->SetText(FText::FromString("Normal"));
						DifficultyTexts[i]->SetColorAndOpacity(FLinearColor::Black);

						break;

					case EDifficulty::ED_Hard:
						DifficultyTexts[i]->SetText(FText::FromString("Hard"));
						DifficultyTexts[i]->SetColorAndOpacity(FLinearColor::Red);

						break;

					default:
						DifficultyTexts[i]->SetText(FText::FromString(""));

					}


					SaveSlotBackgrounds[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
				

				if (bSaveMode)
				{
					if (DeleteButtons.IsValidIndex(i) && DeleteButtons[i])
					{
						DeleteButtons[i]->SetVisibility(ESlateVisibility::Visible);
					}
				}
				else
				{
					if (DeleteButtons.IsValidIndex(i) && DeleteButtons[i])
					{
						DeleteButtons[i]->SetVisibility(ESlateVisibility::Hidden);
					}
				}

			}
			else
			{
				ToDoTexts[i]->SetText(FText::FromString(FString("")));
				SaveSlotBackgrounds[i]->SetVisibility(ESlateVisibility::Hidden);
				TimeTexts[i]->SetText(FText::FromString(FString("")));
				DifficultyTexts[i]->SetText(FText::FromString(""));

				SaveSpotTexts[i]->SetText(FText::FromString(FString("")));

				SaveButtons[i]->SetIsEnabled(bSaveMode);

				if (DeleteButtons.IsValidIndex(i) && DeleteButtons[i])
				{
					DeleteButtons[i]->SetVisibility(ESlateVisibility::Hidden);
				}
				
			}
		}
	}

	if (bSaveMode)
	{
		NewSaveButton->SetIsEnabled(false);
		SaveModeBackGround->SetBrushColor(FLinearColor::Blue);
		SaveModeText->SetText(FText::FromString(FString("Save")));
	}
	else
	{
		NewSaveButton->SetIsEnabled(true);
		SaveModeBackGround->SetBrushColor(FLinearColor::Red);
		SaveModeText->SetText(FText::FromString(FString("Load")));
	}

	ShowCurrentLocation();
}

void USaveWidget::SetMainMenuDisplay(EMenuType menuType, bool bSavePanelSave)
{
	bSaveMode = bSavePanelSave;

	MenuSelectCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SaveShowPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	switch (menuType)
	{
	case EMenuType::EMT_Save :

		SaveCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MapCanvas->SetVisibility(ESlateVisibility::Hidden);

		if (bSaveMode)
		{
			MenuSelectCanvas->SetVisibility(ESlateVisibility::Hidden);
			SaveShowPanel->SetVisibility(ESlateVisibility::Hidden);
		}

		break;

	case EMenuType::EMT_Map:
		MapCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SaveCanvas->SetVisibility(ESlateVisibility::Hidden);

	}

	UpdateAllUI();

}


void USaveWidget::InitArrays()
{
	SaveButtons.Add(SaveButton1);
	SaveButtons.Add(SaveButton2);
	SaveButtons.Add(SaveButton3);
	SaveButtons.Add(SaveButton4);
	SaveButtons.Add(SaveButton5);
	SaveButtons.Add(SaveButton6);
	SaveButtons.Add(SaveButton7);
	SaveButtons.Add(SaveButton8);

	DeleteButtons.Add(deleteButton1);
	DeleteButtons.Add(deleteButton2);
	DeleteButtons.Add(deleteButton3);
	DeleteButtons.Add(deleteButton4);
	DeleteButtons.Add(deleteButton5);
	DeleteButtons.Add(deleteButton6);
	DeleteButtons.Add(deleteButton7);
	DeleteButtons.Add(deleteButton8);

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
