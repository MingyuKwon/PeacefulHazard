// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::BackUIInputTrigger()
{
	if (combineLock)
	{
		combineLock = false;
	}
	else
	{
		InteractLock = false;
	}


	SetInventoryCanvas();
}

void UInventoryWidget::SetInventoryDisplay(FCharacterInventoty* inventory)
{
	recentinventory = inventory;

	for (int i = 0; i < 15; i++)
	{
		FText TempText;

		if (recentinventory->ItemLockArray[i])
		{
			ItemButtons[i]->SetVisibility(ESlateVisibility::Hidden);
			ItemImages[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ItemButtons[i]->SetVisibility(ESlateVisibility::Visible);
			ItemImages[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}


		if (recentinventory->inventoryItemCounts[i] == 0)
		{
			ItemCountBorders[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ItemCountBorders[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			TempText = FText::AsNumber(recentinventory->inventoryItemCounts[i]);
			ItemCountTexts[i]->SetText(TempText);

			if (recentinventory->inventoryItemCounts[i] == ItemInformation->ItemInformationMap[recentinventory->inventoryItems[i]].itemMaxCount)
			{
				ItemCountTexts[i]->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
			}
			else
			{
				ItemCountTexts[i]->SetColorAndOpacity(FSlateColor(FLinearColor::White));
			}

		}

		if (ItemInformation)
		{
			UTexture2D* Texture = ItemInformation->ItemInformationMap[recentinventory->inventoryItems[i]].itemImage;
			if (Texture)
			{
				ItemImages[i]->SetBrushFromTexture(Texture);
			}
		}
	}

	SetInventoryCanvas();

}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));


	InitArrays();

	for (UButton* Button : ItemButtons)
	{
		if (Button)
		{
			Button->OnClicked.AddDynamic(this, &ThisClass::OnItemButtonClicked);
			Button->OnHovered.AddDynamic(this, &ThisClass::OnItemButtonHovered);
			Button->OnUnhovered.AddDynamic(this, &ThisClass::OnItemButtonUnhovered);
		}
	}

	if (UseButton)
	{
		UseButton->OnClicked.AddDynamic(this, &ThisClass::OnUseButtoClicked);
	}
	if (CombineButton)
	{
		CombineButton->OnClicked.AddDynamic(this, &ThisClass::OnCombineButtoClicked);
	}
	if (DiscardButton)
	{
		DiscardButton->OnClicked.AddDynamic(this, &ThisClass::OnDiscardButtoClicked);
	}

	UIInteractCanvas->SetVisibility(ESlateVisibility::Hidden);

}

void UInventoryWidget::OnUseButtoClicked()
{

}

void UInventoryWidget::OnCombineButtoClicked()
{
	combineLock = true;

	SetInventoryCanvas();
}

void UInventoryWidget::OnDiscardButtoClicked()
{
	if (NowHoveringButton == nullptr) return;

	int32 index = GetButtonIndex(NowHoveringButton);
	EItemType itemType = recentinventory->inventoryItems[index];
	int32 itemCount = recentinventory->inventoryItemCounts[index];

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(GetButtonIndex(NowHoveringButton), itemType, -1, false);
	}
}


bool UInventoryWidget::CanInteractButton(UButton* button)
{
	int32 index = GetButtonIndex(button);

	if (index >= 0)
	{
		if (recentinventory->inventoryItems[index] != EItemType::EIT_None)
		{
			return true;
		}
		
	}

	return false;
}

int32 UInventoryWidget::GetButtonIndex(UButton* button)
{
	int32 index = 0;

	for (UButton* Button : ItemButtons)
	{
		if (Button == button)
		{
			break;
		}

		index++;
	}

	return index < 15 ? index : -1;
}

void UInventoryWidget::ChangeNowHoveringButton(UButton* button)
{
	if (InteractLock) return;

	NowHoveringButton = button;

	SetInventoryCanvas();
}

void UInventoryWidget::OnItemButtonClicked()
{
	if (!CanInteractButton(NowHoveringButton)) return;

	InteractLock = true;

	SetInventoryCanvas();
}

void UInventoryWidget::OnItemButtonHovered()
{
	UButton* FindButton = nullptr;

	for (UButton* Button : ItemButtons)
	{
		if (Button->IsHovered())
		{
			FindButton = Button;
			break;
		}
	}

	ChangeNowHoveringButton(FindButton);
}

void UInventoryWidget::OnItemButtonUnhovered()
{
	ChangeNowHoveringButton(nullptr);
	
}

void UInventoryWidget::SetInventoryCanvas()
{
	SetItemExplainText();
	SetInteractPanelButton();

	if (NowHoveringButton == nullptr)
	{
		UIInteractCanvas->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	UIInteractCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (UCanvasPanelSlot* ButtonSlot = Cast<UCanvasPanelSlot>(NowHoveringButton->Slot))
	{
		FVector2D ButtonPosition = ButtonSlot->GetPosition();

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(UIInteractCanvas->Slot))
		{
			CanvasSlot->SetPosition(ButtonPosition);
		}
	}


	if (recentinventory->inventoryItems[GetButtonIndex(NowHoveringButton)] == EItemType::EIT_None)
	{
		InteractLock = false;
	}

	if (InteractLock)
	{
		if (combineLock)
		{
			CombineModeBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			InteractBackGround->SetVisibility(ESlateVisibility::Hidden);
			UseButton->SetVisibility(ESlateVisibility::Hidden);
			CombineButton->SetVisibility(ESlateVisibility::Hidden);
			DiscardButton->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			CombineModeBorder->SetVisibility(ESlateVisibility::Hidden);
			InteractBackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UseButton->SetVisibility(ESlateVisibility::Visible);
			CombineButton->SetVisibility(ESlateVisibility::Visible);
			DiscardButton->SetVisibility(ESlateVisibility::Visible);

		}


	}
	else
	{
		CombineModeBorder->SetVisibility(ESlateVisibility::Hidden);
		InteractBackGround->SetVisibility(ESlateVisibility::Hidden);
		UseButton->SetVisibility(ESlateVisibility::Hidden);
		CombineButton->SetVisibility(ESlateVisibility::Hidden);
		DiscardButton->SetVisibility(ESlateVisibility::Hidden);

	}
}

void UInventoryWidget::SetItemExplainText()
{
	FText TempText = FText();

	if (NowHoveringButton == nullptr)
	{
		ItemNameText->SetText(TempText);
		ItemExplainText->SetText(TempText);
	}
	else
	{
		int32 index = GetButtonIndex(NowHoveringButton);


		if (ItemInformation)
		{
			FString string = ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]].itemExplainText;
			TempText = FText::FromString(string);
			ItemExplainText->SetText(TempText);

			string = ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]].itemNameText;
			TempText = FText::FromString(string);
			ItemNameText->SetText(TempText);

		}


	}
}

void UInventoryWidget::SetInteractPanelButton()
{
	if (NowHoveringButton == nullptr) return;

	int32 index = GetButtonIndex(NowHoveringButton);

	if (ItemInformation)
	{
		if (ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]].bUsable)
		{
			UseButton->SetIsEnabled(true);
		}
		else
		{
			UseButton->SetIsEnabled(false);
		}

		if (ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]].ItemCombineArray.Num() > 0)
		{
			CombineButton->SetIsEnabled(true);
		}
		else
		{
			CombineButton->SetIsEnabled(false);
		}

		if (ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]].bDiscardable)
		{
			DiscardButton->SetIsEnabled(true);
		}
		else
		{
			DiscardButton->SetIsEnabled(false);
		}

	}
}

void UInventoryWidget::InitArrays()
{
	ItemButtons.Add(ItemButton1);
	ItemButtons.Add(ItemButton2);
	ItemButtons.Add(ItemButton3);
	ItemButtons.Add(ItemButton4);
	ItemButtons.Add(ItemButton5);
	ItemButtons.Add(ItemButton6);
	ItemButtons.Add(ItemButton7);
	ItemButtons.Add(ItemButton8);
	ItemButtons.Add(ItemButton9);
	ItemButtons.Add(ItemButton10);
	ItemButtons.Add(ItemButton11);
	ItemButtons.Add(ItemButton12);
	ItemButtons.Add(ItemButton13);
	ItemButtons.Add(ItemButton14);
	ItemButtons.Add(ItemButton15);

	ItemImages.Add(ItemImage1);
	ItemImages.Add(ItemImage2);
	ItemImages.Add(ItemImage3);
	ItemImages.Add(ItemImage4);
	ItemImages.Add(ItemImage5);
	ItemImages.Add(ItemImage6);
	ItemImages.Add(ItemImage7);
	ItemImages.Add(ItemImage8);
	ItemImages.Add(ItemImage9);
	ItemImages.Add(ItemImage10);
	ItemImages.Add(ItemImage11);
	ItemImages.Add(ItemImage12);
	ItemImages.Add(ItemImage13);
	ItemImages.Add(ItemImage14);
	ItemImages.Add(ItemImage15);

	ItemCountTexts.Add(ItemCountText1);
	ItemCountTexts.Add(ItemCountText2);
	ItemCountTexts.Add(ItemCountText3);
	ItemCountTexts.Add(ItemCountText4);
	ItemCountTexts.Add(ItemCountText5);
	ItemCountTexts.Add(ItemCountText6);
	ItemCountTexts.Add(ItemCountText7);
	ItemCountTexts.Add(ItemCountText8);
	ItemCountTexts.Add(ItemCountText9);
	ItemCountTexts.Add(ItemCountText10);
	ItemCountTexts.Add(ItemCountText11);
	ItemCountTexts.Add(ItemCountText12);
	ItemCountTexts.Add(ItemCountText13);
	ItemCountTexts.Add(ItemCountText14);
	ItemCountTexts.Add(ItemCountText15);

	ItemCountBorders.Add(ItemCountBorder1);
	ItemCountBorders.Add(ItemCountBorder2);
	ItemCountBorders.Add(ItemCountBorder3);
	ItemCountBorders.Add(ItemCountBorder4);
	ItemCountBorders.Add(ItemCountBorder5);
	ItemCountBorders.Add(ItemCountBorder6);
	ItemCountBorders.Add(ItemCountBorder7);
	ItemCountBorders.Add(ItemCountBorder8);
	ItemCountBorders.Add(ItemCountBorder9);
	ItemCountBorders.Add(ItemCountBorder10);
	ItemCountBorders.Add(ItemCountBorder11);
	ItemCountBorders.Add(ItemCountBorder12);
	ItemCountBorders.Add(ItemCountBorder13);
	ItemCountBorders.Add(ItemCountBorder14);
	ItemCountBorders.Add(ItemCountBorder15);
}

