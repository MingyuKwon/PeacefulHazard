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

	if (PeaceFulHazardGameMode == nullptr) return;

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

				UPeacFulSaveGame* LoadedGame = Cast<UPeacFulSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
				
				if (PeacFulGameInstance)
				{
					FString temp = PeacFulGameInstance->ToDoMap[PeacFulGameInstance->currentToDos[LoadedGame->saveTodoIndex]];
					ToDoTexts[i]->SetText(FText::FromString(temp));

					TimeTexts[i]->SetText(FText::FromString(LoadedGame->SaveTime.ToString(TEXT("%m/%d  |  %H:%M"))));

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

}
