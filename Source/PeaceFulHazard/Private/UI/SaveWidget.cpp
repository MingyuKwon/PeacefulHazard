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

void USaveWidget::OnSaveButtonClicked()
{
	if (!bSaveMode) return;

}

void USaveWidget::OnSaveButtonHovered()
{
	if (!bSaveMode) return;

}

void USaveWidget::OnSaveButtonUnhovered()
{
	if (!bSaveMode) return;

}

void USaveWidget::OnDeleteButtonClicked()
{
	if (bSaveMode) return;

}

void USaveWidget::OnDeleteButtonHovered()
{
	if (bSaveMode) return;

}

void USaveWidget::OnDeleteButtonUnhovered()
{
	if (bSaveMode) return;

}

void USaveWidget::OnNewButtonClicked()
{
	if (bSaveMode) return;

}

void USaveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

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
			SaveButtons[i]->bIsEnabled = true;

			FString SlotName = SaveButtons[i]->GetName();

			if (UGameplayStatics::DoesSaveGameExist(SlotName, 0) && bSaveMode)
			{
				if (DeleteButtons.IsValidIndex(i) && DeleteButtons[i])
				{
					DeleteButtons[i]->SetVisibility(ESlateVisibility::Visible);
				}
			}
			else
			{
				SaveButtons[i]->bIsEnabled = bSaveMode;


				if (DeleteButtons.IsValidIndex(i) && DeleteButtons[i])
				{
					DeleteButtons[i]->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}

	if (bSaveMode)
	{
		NewSaveButton->SetVisibility(ESlateVisibility::Hidden);
		SaveModeBackGround->SetBrushColor(FLinearColor::Blue);
		SaveModeText->SetText(FText::FromString(FString("Save")));
	}
	else
	{
		NewSaveButton->SetVisibility(ESlateVisibility::Visible);
		SaveModeBackGround->SetBrushColor(FLinearColor::Red);
		SaveModeText->SetText(FText::FromString(FString("Load")));


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
