// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenu_SettingWidget.h"
#include "MainMenu/MainMenuGameMode.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"

#include "Components/CanvasPanelSlot.h"

#include "System/PeacFulGameInstance.h"

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
	PeacFulGameInstance = Cast<UPeacFulGameInstance>(GetGameInstance());

	LoadButton->OnClicked.AddDynamic(this, &ThisClass::OnLoadButtonClicked);
	NewGaemButton->OnClicked.AddDynamic(this, &ThisClass::OnNewGameButtonClicked);

	LoadButton->OnHovered.AddDynamic(this, &ThisClass::OnLoadButtonHovered);
	NewGaemButton->OnHovered.AddDynamic(this, &ThisClass::OnNewGameButtonHoverd);

	if (MainMenuGameMode)
	{
		MainMenuGameMode->LanguageChangeEvent.AddDynamic(this, &ThisClass::UpdateAllUI);
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

void UMainMenu_SettingWidget::OnLoadButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Load, true);

		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);

	}
}

void UMainMenu_SettingWidget::OnNewGameButtonClicked()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_NewGame, true);
		MainMenuGameMode->PlayUISound(ButtonClickSound, 0.7f);

	}
}

void UMainMenu_SettingWidget::OnLoadButtonHovered()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);

	}
}

void UMainMenu_SettingWidget::OnNewGameButtonHoverd()
{
	if (MainMenuGameMode)
	{
		MainMenuGameMode->PlayUISound(ButtonHoverSound, 0.5f);

	}
}

void UMainMenu_SettingWidget::CheckLanguage()
{
	if (MainMenuGameMode == nullptr) return;

	SetLangaugeText(MainMenuGameMode->GetCurrentLanguage());

}

void UMainMenu_SettingWidget::OnResolutionDropDownChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (PeacFulGameInstance)
	{
		PeacFulGameInstance->SetResolution(SelectedItem);
	}

}

void UMainMenu_SettingWidget::OnLanguageDropDownChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (PeacFulGameInstance)
	{
		PeacFulGameInstance->SetLangauage(SelectedItem);
	}
}

void UMainMenu_SettingWidget::OnDefaultMouseSensibilityValueChanged(float Value)
{
	if (DefaultMouseSensibility && DefaultMouseSensibilityFill)
	{
		if (GEngine)
		{
			FString text = FString::Printf(TEXT("OnDefaultMouseSensibilityValueChanged : %f "), Value);
			GEngine->AddOnScreenDebugMessage(8, 1.f, FColor::Green, text);
		}

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

void UMainMenu_SettingWidget::OnAimMouseSensibilityValueChanged(float Value)
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

void UMainMenu_SettingWidget::OnBGMChanged(float Value)
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

		if (MainMenuGameMode)
		{
			MainMenuGameMode->SetBGMVolume(Value);
		}


	}
}

void UMainMenu_SettingWidget::OnSFXChanged(float Value)
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

void UMainMenu_SettingWidget::OnUIChanged(float Value)
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

void UMainMenu_SettingWidget::OnBrightChanged(float Value)
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

		if (MainMenuGameMode)
		{
			MainMenuGameMode->SetGameBrightness();
		}
	}
}

void UMainMenu_SettingWidget::OnResetSettingValue()
{
	if (PeacFulGameInstance)
	{
		PeacFulGameInstance->ResetSetting();
	}

	UpdateAllUI();
}

void UMainMenu_SettingWidget::UpdateAllUI()
{
	CheckLanguage();

	if (PeacFulGameInstance)
	{

		if (DefaultMouseSensibility)
		{
			DefaultMouseSensibility->SetValue(PeacFulGameInstance->MouseSensitivity);
			OnDefaultMouseSensibilityValueChanged(PeacFulGameInstance->MouseSensitivity);

		}

		if (AimMouseSensibility)
		{
			AimMouseSensibility->SetValue(PeacFulGameInstance->MouseAimSensitivity);
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
}
