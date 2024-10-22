﻿// Fill out your copyright notice in the Description page of Project Settings.


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
#include "Components/ComboBoxString.h"
#include "SlateCore.h"
#include "System/PeacFulSaveGame.h"

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

		CheckCallBackHoveringSaveButtonName = HoveringSaveButton->GetName();

		if (!CheckCallBackHoveringSaveButtonName.IsEmpty())
		{
			FString EnglishMessage = bSaveMode
				? FString::Printf(TEXT("Do you want to save in slot \"%s\"?"), *CheckCallBackHoveringSaveButtonName)
				: FString::Printf(TEXT("Do you want to load from slot \"%s\"?"), *CheckCallBackHoveringSaveButtonName);

			FString KoreanMessage = bSaveMode
				? FString::Printf(TEXT("\"%s\" 슬롯에 저장하시겠습니까?"), *CheckCallBackHoveringSaveButtonName)
				: FString::Printf(TEXT("\"%s\" 슬롯을 불러오겠습니까?"), *CheckCallBackHoveringSaveButtonName);

			PeaceFulHazardGameMode->CheckOneMoreGameEvent.Broadcast(true, FText::FromString(EnglishMessage), FText::FromString(KoreanMessage));
			PeaceFulHazardGameMode->CheckOneMoreSuccessGameEvent.AddDynamic(this, &ThisClass::OnceSaveButtonClickedSuccess);
		}		

	}

}

void USaveWidget::OnceSaveButtonClickedSuccess()
{
	if (CheckCallBackHoveringSaveButtonName.IsEmpty()) return;

	PeaceFulHazardGameMode->CheckOneMoreSuccessGameEvent.RemoveDynamic(this, &ThisClass::OnceSaveButtonClickedSuccess);

	if (bSaveMode)
	{
		PeaceFulHazardGameMode->SaveDataToSlot(CheckCallBackHoveringSaveButtonName);
	}
	else
	{
		PeaceFulHazardGameMode->LoadDataFromSlot(CheckCallBackHoveringSaveButtonName, false);
	}
	CheckCallBackHoveringSaveButtonName = FString("");

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

		CheckCallBackHoveringDeleteButtonindex = index;

		FString ButtonName = SaveButtons[CheckCallBackHoveringDeleteButtonindex]->GetName();
		FString EnglishMessage = FString::Printf(TEXT("Do you want to delete slot \"%s\"?"), *ButtonName);
		FString KoreanMessage = FString::Printf(TEXT("\"%s\" 슬롯을 삭제 하시겠습니까?"), *ButtonName);

		PeaceFulHazardGameMode->CheckOneMoreGameEvent.Broadcast(true, FText::FromString(EnglishMessage), FText::FromString(KoreanMessage));
		PeaceFulHazardGameMode->CheckOneMoreSuccessGameEvent.AddDynamic(this, &ThisClass::OnceDeleteButtonClickedSuccess);

	}

	
}

void USaveWidget::OnceDeleteButtonClickedSuccess()
{
	PeaceFulHazardGameMode->CheckOneMoreSuccessGameEvent.RemoveDynamic(this, &ThisClass::OnceDeleteButtonClickedSuccess);

	if (CheckCallBackHoveringDeleteButtonindex >= 0 && CheckCallBackHoveringDeleteButtonindex < 8)
	{
		if (bSaveMode)
		{
			PeaceFulHazardGameMode->DeleteDataFromSlot(SaveButtons[CheckCallBackHoveringDeleteButtonindex]->GetName());
		}
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
		PeaceFulHazardGameMode->CheckOneMoreGameEvent.Broadcast(true, FText::FromString(FString("Play a new game? \n(Difficulty will not change)")), FText::FromString(FString(TEXT("새 게임을 시작하시겠습니까? \n(난이도는 유지 됩니다)"))));
		PeaceFulHazardGameMode->CheckOneMoreSuccessGameEvent.AddDynamic(this, &ThisClass::OnceNewButtonClickedSuccess);

	}


}

void USaveWidget::OnNewButtonHovered()
{
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonHoverSound, 1.f);
	}
}

void USaveWidget::OnceNewButtonClickedSuccess()
{
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->CheckOneMoreSuccessGameEvent.RemoveDynamic(this, &ThisClass::OnceNewButtonClickedSuccess);

		PeaceFulHazardGameMode->LoadDataFromSlot(FString(""), true);

	}

}

void USaveWidget::CheckLanguage()
{
	if (PeaceFulHazardGameMode == nullptr) return;
	
	SetLangaugeText(PeaceFulHazardGameMode->GetCurrentLanguage());
}

void USaveWidget::SetDynamicChangeLanguage(UTextBlock* textBlock, const FText& Englishtext, const FText& Koreantext)
{
	if (PeaceFulHazardGameMode == nullptr) return;

	switch (PeaceFulHazardGameMode->GetCurrentLanguage())
	{
	case ELanguage::ED_English :
		textBlock->SetText(Englishtext);
		break;

	case ELanguage::ED_Korean:
		textBlock->SetText(Koreantext);
		break;

	}
}


void USaveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->SaveFinishedEvent.AddDynamic(this, &ThisClass::UpdateAllUI);
		PeaceFulHazardGameMode->LanguageChangeEvent.AddDynamic(this, &ThisClass::UpdateAllUI);
	}

	InitArrays();

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
		NewSaveButton->OnHovered.AddDynamic(this, &ThisClass::OnNewButtonHovered);

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



	if (SettingResetButton)
	{
		SettingResetButton->OnClicked.AddDynamic(this, &ThisClass::OnResetSettingValue);
	}

	if (DefaultMouseSensibility)
	{
		DefaultMouseSensibility->OnValueChanged.AddDynamic(this, &ThisClass::OnDefaultMouseSensibilityValueChanged);
	}

	if (AimMouseSensibility)
	{
		AimMouseSensibility->OnValueChanged.AddDynamic(this, &ThisClass::OnAimMouseSensibilityValueChanged);
	}

	if (BGMVolumue)
	{
		BGMVolumue->OnValueChanged.AddDynamic(this, &ThisClass::OnBGMChanged);
	}

	if (SFXVolumue)
	{
		SFXVolumue->OnValueChanged.AddDynamic(this, &ThisClass::OnSFXChanged);
	}

	if (UIVolumue)
	{
		UIVolumue->OnValueChanged.AddDynamic(this, &ThisClass::OnUIChanged);
	}

	if (Brightness)
	{
		Brightness->OnValueChanged.AddDynamic(this, &ThisClass::OnBrightChanged);
	}

	if (ResolutionDropDown)
	{
		ResolutionDropDown->OnSelectionChanged.AddDynamic(this, &ThisClass::OnResolutionDropDownChanged);
	}

	if (LanguageDropDown)
	{
		LanguageDropDown->OnSelectionChanged.AddDynamic(this, &ThisClass::OnLanguageDropDownChanged);
	}
	
	UpdateAllUI();
}

void USaveWidget::OnResolutionDropDownChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (PeacFulGameInstance)
	{
		PeacFulGameInstance->SetResolution(SelectedItem);
	}
}

void USaveWidget::OnLanguageDropDownChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (PeacFulGameInstance)
	{
		PeacFulGameInstance->SetLangauage(SelectedItem);
	}
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

void USaveWidget::OnBGMChanged(float Value)
{
	if (BGMVolumue && BGMVolumueFill)
	{
		UCanvasPanelSlot* CanvasSlotFill = Cast<UCanvasPanelSlot>(BGMVolumueFill->Slot);

		if (CanvasSlotFill)
		{
			FMargin NewOffsets = CanvasSlotFill->GetOffsets();
			NewOffsets.Right = maxVolumeFill * (1 - Value);

			CanvasSlotFill->SetOffsets(NewOffsets);

		}

		if (PeacFulGameInstance)
		{
			PeacFulGameInstance->BGMVolume = Value;
		}

		if (PeaceFulHazardGameMode)
		{
			PeaceFulHazardGameMode->SetBGMVolume(Value);
		}
		

	}

}

void USaveWidget::OnSFXChanged(float Value)
{
	if (SFXVolumue && SFXVolumueFill)
	{
		UCanvasPanelSlot* CanvasSlotFill = Cast<UCanvasPanelSlot>(SFXVolumueFill->Slot);

		if (CanvasSlotFill)
		{
			FMargin NewOffsets = CanvasSlotFill->GetOffsets();
			NewOffsets.Right = maxVolumeFill * (1 - Value);

			CanvasSlotFill->SetOffsets(NewOffsets);

		}

		if (PeacFulGameInstance)
		{
			PeacFulGameInstance->SFXVolume = Value;
		}
	}
}

void USaveWidget::OnUIChanged(float Value)
{
	if (UIVolumue && UIVolumueFill)
	{
		UCanvasPanelSlot* CanvasSlotFill = Cast<UCanvasPanelSlot>(UIVolumueFill->Slot);

		if (CanvasSlotFill)
		{
			FMargin NewOffsets = CanvasSlotFill->GetOffsets();
			NewOffsets.Right = maxVolumeFill * (1 - Value);

			CanvasSlotFill->SetOffsets(NewOffsets);

		}

		if (PeacFulGameInstance)
		{
			PeacFulGameInstance->UIVolume = Value;
		}
	}
}

void USaveWidget::OnBrightChanged(float Value)
{
	if (Brightness && BrightnessFill)
	{
		UCanvasPanelSlot* CanvasSlotFill = Cast<UCanvasPanelSlot>(BrightnessFill->Slot);

		if (CanvasSlotFill)
		{
			FMargin NewOffsets = CanvasSlotFill->GetOffsets();
			NewOffsets.Right = maxBrightnessFill * (1 - Value);

			CanvasSlotFill->SetOffsets(NewOffsets);

		}

		if (PeacFulGameInstance)
		{
			PeacFulGameInstance->Brightness = Value * 4 + 11.f;
		}

		if (PeaceFulHazardGameMode)
		{
			PeaceFulHazardGameMode->SetGameBrightness();
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
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);

		PeaceFulHazardGameMode->CheckOneMoreGameEvent.Broadcast(true, FText::FromString(FString("Do you want to go back to Title Menu? \n(Unsaved Data will be deleted)")), FText::FromString(FString(TEXT("타이틀로 돌아가시겠습니까? \n(저장되지 않은 정보는 없어집니다)"))));
		PeaceFulHazardGameMode->CheckOneMoreSuccessGameEvent.AddDynamic(this, &ThisClass::OnceExitButtonSuccess);
	}
}

void USaveWidget::OnceExitButtonSuccess()
{
	PeaceFulHazardGameMode->CheckOneMoreSuccessGameEvent.RemoveDynamic(this, &ThisClass::OnceExitButtonSuccess);

	PeaceFulHazardGameMode->ShowLoadingEvent.Broadcast(true);
	PeaceFulHazardGameMode->MoveToMainMenu();


}



void USaveWidget::ShowCurrentLocation()
{
	if (PeaceFulHazardGameMode == nullptr) return;

	switch (PeaceFulHazardGameMode->GetDifficulty())
	{
	case EDifficulty::ED_Easy:
		SetDynamicChangeLanguage(MapDifficulty, FText::FromString("Easy"), FText::FromString(TEXT("쉬움")));
		MapDifficulty->SetColorAndOpacity(FLinearColor::Green);
		break;

	case EDifficulty::ED_Normal:
		SetDynamicChangeLanguage(MapDifficulty, FText::FromString("Normal"), FText::FromString(TEXT("보통")));
		MapDifficulty->SetColorAndOpacity(FLinearColor::Black);

		break;

	case EDifficulty::ED_Hard:
		SetDynamicChangeLanguage(MapDifficulty, FText::FromString("Hard"), FText::FromString(TEXT("어려움")));
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
	CheckLanguage();
	UpdaetLeftKeyDoorCount();

	if (PeaceFulHazardGameMode)
	{
		float defaultMouse;
		float aimMouse;
		PeaceFulHazardGameMode->GetSettingValue(defaultMouse, aimMouse);

		if (DefaultMouseSensibility)
		{
			DefaultMouseSensibility->SetValue(defaultMouse);
			OnDefaultMouseSensibilityValueChanged(defaultMouse);
		}

		if (AimMouseSensibility)
		{
			AimMouseSensibility->SetValue(aimMouse);
		}

		if (PeacFulGameInstance)
		{
			if (BGMVolumue)
			{
				BGMVolumue->SetValue(PeacFulGameInstance->BGMVolume);
			}

			if (SFXVolumue)
			{
				SFXVolumue->SetValue(PeacFulGameInstance->SFXVolume);
			}

			if (UIVolumue)
			{
				UIVolumue->SetValue(PeacFulGameInstance->UIVolume);
			}
			
			if (Brightness)
			{
				Brightness->SetValue((PeacFulGameInstance->Brightness - 11.f) / 4.f);
			}

			if (ResolutionDropDown)
			{
				ResolutionDropDown->SetSelectedOption(PeacFulGameInstance->Resolution);
			}

			if (LanguageDropDown)
			{
				LanguageDropDown->SetSelectedOption(PeacFulGameInstance->Language);
			}
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
					FString temp = PeacFulGameInstance->GetToDoMapByLanguage()[PeacFulGameInstance->currentToDos[LoadedGame->saveTodoIndex]];
					ToDoTexts[i]->SetText(FText::FromString(temp));

					TimeTexts[i]->SetText(FText::FromString(LoadedGame->SaveTime.ToString(TEXT("%m/%d  |  %H:%M"))));

					SaveSpotTexts[i]->SetText(FText::FromString(PeacFulGameInstance->GetMapNameByLanguage()[LoadedGame->saveMapName]));

					switch (LoadedGame->SavegameDifficulty)
					{
					case EDifficulty::ED_Easy :
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
		SetDynamicChangeLanguage(SaveModeText, FText::FromString("Save"), FText::FromString(TEXT("저장")));

	}
	else
	{
		NewSaveButton->SetIsEnabled(true);
		SaveModeBackGround->SetBrushColor(FLinearColor::Red);
		SetDynamicChangeLanguage(SaveModeText, FText::FromString("Load"), FText::FromString(TEXT("불러오기")));

	}

	ShowCurrentLocation();
}

void USaveWidget::SetMainMenuDisplay(EMenuType menuType, bool bSavePanelSave)
{
	bSaveMode = bSavePanelSave;

	MenuSelectCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SaveShowPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OptionCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	switch (menuType)
	{
	case EMenuType::EMT_Save :

		SaveCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MapCanvas->SetVisibility(ESlateVisibility::Hidden);
		OptionCanvas->SetVisibility(ESlateVisibility::Hidden);

		if (bSaveMode)
		{
			MenuSelectCanvas->SetVisibility(ESlateVisibility::Hidden);
			SaveShowPanel->SetVisibility(ESlateVisibility::Hidden);
		}

		break;

	case EMenuType::EMT_Map:
		MapCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SaveCanvas->SetVisibility(ESlateVisibility::Hidden);
		OptionCanvas->SetVisibility(ESlateVisibility::Hidden);

		break;

	case EMenuType::EMT_Option:
		MapCanvas->SetVisibility(ESlateVisibility::Hidden);
		SaveCanvas->SetVisibility(ESlateVisibility::Hidden);
		OptionCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		break;
	}

	UpdateAllUI();

}



void USaveWidget::UpdaetLeftKeyDoorCount()
{
	if (PeacFulGameInstance == nullptr) return;

	UPeacFulSaveGame* tempSave = PeacFulGameInstance->tempSaveGame;

	int32 totalNormalKeyCount = 11;
	int32 totalBigKeyCount = 2;

	int32 totalNormalDoorCount = 7;
	int32 totalBigDoorCount = 4;



	int32 localNormalDoorCount = 1;
	int32 localBigDoorCount = 1;
	EWarpTarget currentMapType = EWarpTarget::EWT_Tutorial;
	TArray<FString> tempInteractMap = tempSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames;
	IterateStringArray(tempInteractMap, localNormalDoorCount, localBigDoorCount, totalNormalDoorCount, totalBigDoorCount, totalNormalKeyCount, totalBigKeyCount);
	LeftNormalDoorTutorialCountText->SetText(FText::AsNumber(localNormalDoorCount));
	LeftBigDoorTutorialCountText->SetText(FText::AsNumber(localBigDoorCount));


	localNormalDoorCount = 2;
	localBigDoorCount = 2;
	currentMapType = EWarpTarget::EWT_MainHub;
	tempInteractMap = tempSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames;
	IterateStringArray(tempInteractMap, localNormalDoorCount, localBigDoorCount, totalNormalDoorCount, totalBigDoorCount, totalNormalKeyCount, totalBigKeyCount);
	LeftNormalDoorMainHubCountText->SetText(FText::AsNumber(localNormalDoorCount));
	LeftBigDoorMainhubCountText->SetText(FText::AsNumber(localBigDoorCount));


	localNormalDoorCount = 0;
	localBigDoorCount = 0;
	currentMapType = EWarpTarget::EWT_MainCathedral;
	tempInteractMap = tempSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames;
	IterateStringArray(tempInteractMap, localNormalDoorCount, localBigDoorCount, totalNormalDoorCount, totalBigDoorCount, totalNormalKeyCount, totalBigKeyCount);


	localNormalDoorCount = 1;
	localBigDoorCount = 0;
	currentMapType = EWarpTarget::EWT_CrossOver;
	tempInteractMap = tempSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames;
	IterateStringArray(tempInteractMap, localNormalDoorCount, localBigDoorCount, totalNormalDoorCount, totalBigDoorCount, totalNormalKeyCount, totalBigKeyCount);
	LeftNormalDoorCrossoverCountText->SetText(FText::AsNumber(localNormalDoorCount));
	LeftBigDoorCrossoverCountText->SetText(FText::AsNumber(localBigDoorCount));

	localNormalDoorCount = 1;
	localBigDoorCount = 0;
	currentMapType = EWarpTarget::EWT_GraveYard;
	tempInteractMap = tempSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames;
	IterateStringArray(tempInteractMap, localNormalDoorCount, localBigDoorCount, totalNormalDoorCount, totalBigDoorCount, totalNormalKeyCount, totalBigKeyCount);
	LeftNormalDoorGraveyardCountText->SetText(FText::AsNumber(localNormalDoorCount));
	LeftBigDoorGraveyardCountText->SetText(FText::AsNumber(localBigDoorCount));

	localNormalDoorCount = 1;
	localBigDoorCount = 1;
	currentMapType = EWarpTarget::EWT_RightGarden;
	tempInteractMap = tempSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames;
	IterateStringArray(tempInteractMap, localNormalDoorCount, localBigDoorCount, totalNormalDoorCount, totalBigDoorCount, totalNormalKeyCount, totalBigKeyCount);
	LeftNormalDoorRightCountText->SetText(FText::AsNumber(localNormalDoorCount));
	LeftBigDoorRightCountText->SetText(FText::AsNumber(localBigDoorCount));

	localNormalDoorCount = 1;
	localBigDoorCount = 0;
	currentMapType = EWarpTarget::EWT_LeftGarden;
	tempInteractMap = tempSave->MapInteractSaveMap.FindOrAdd(currentMapType).interactedItemNames;
	IterateStringArray(tempInteractMap, localNormalDoorCount, localBigDoorCount, totalNormalDoorCount, totalBigDoorCount, totalNormalKeyCount, totalBigKeyCount);
	LeftNormalDoorLeftCountText->SetText(FText::AsNumber(localNormalDoorCount));
	LeftBigDoorLeftCountText->SetText(FText::AsNumber(localBigDoorCount));



	LeftNormalKeyTotalCountText->SetText(FText::AsNumber(totalNormalKeyCount));
	LeftBigKeyTotalCountText->SetText(FText::AsNumber(totalBigKeyCount));

	LeftNormalDoorTotalCountText->SetText(FText::AsNumber(totalNormalDoorCount));
	LeftBiglDoorTotalCountText->SetText(FText::AsNumber(totalBigDoorCount));


}

void USaveWidget::IterateStringArray(TArray<FString> tempInteractMap, int32& localNormalDoorCount, int32& localBigDoorCount, int32& totalNormalDoorCount, int32& totalBigDoorCount, int32& totalNormalKeyCount, int32& totalBigKeyCount)
{
	for (FString str : tempInteractMap)
	{
		if (str.Contains(FString("NormalDoor")))
		{
			localNormalDoorCount--;

			totalNormalDoorCount--;
		}

		if (str.Contains(FString("BigDoor")))
		{
			localBigDoorCount--;

			totalBigDoorCount--;
		}

		if (str.Contains(FString("NormalKey")))
		{
			totalNormalKeyCount--;
		}

		if (str.Contains(FString("BigKey")))
		{
			totalBigKeyCount--;
		}



	}

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
