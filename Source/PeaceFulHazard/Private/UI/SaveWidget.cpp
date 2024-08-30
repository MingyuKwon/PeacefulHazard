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

	if (HoveringSaveButton == nullptr) return;
	if (PeaceFulHazardGameMode == nullptr) return;

	PeaceFulHazardGameMode->LoadDataFromSlot(HoveringSaveButton->GetName(), true);

}

void USaveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->SaveFinishedEvent.AddDynamic(this, &ThisClass::UpdateAllUI);
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
	}
	
	UpdateAllUI();
}

void USaveWidget::UpdateAllUI()
{
	for (int32 i = 0; i < SaveButtons.Num(); i++)
	{
		if (SaveButtons[i])
		{
			SaveButtons[i]->SetIsEnabled(true);

			FString SlotName = SaveButtons[i]->GetName();

			if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
			{
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


	if (HoveringSaveButton == nullptr)
	{
		UISaveInteractCanvas->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (UCanvasPanelSlot* ButtonSlot = Cast<UCanvasPanelSlot>(HoveringSaveButton->Slot))
	{
		UISaveInteractCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FVector2D ButtonPosition = ButtonSlot->GetPosition();

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(UISaveInteractCanvas->Slot))
		{
			CanvasSlot->SetPosition(ButtonPosition);
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

}
