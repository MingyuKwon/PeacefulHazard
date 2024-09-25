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
	if (GetVisibility() == ESlateVisibility::Hidden) return;


	if (combineLock)
	{
		combineLock = false;
	}
	else if (MoveLock)
	{
		MoveLock = false;
	}
	else if(InteractLock)
	{
		InteractLock = false;
		NowInteractButton = nullptr;
	}
	else
	{
		if (PeaceFulHazardGameMode)
		{
			situationLock = false;
			PeaceFulHazardGameMode->CloseAllUIEvent.Broadcast();
		}

		return;
	}

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(BackSound, 0.3f);
	}


	SetAllUIUpdate();

}

void UInventoryWidget::OkUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (beforeitemType == EItemType::EIT_None) return;
	if (TakeImage->GetVisibility() == ESlateVisibility::Hidden) return;

	if (ItemGetCanvas->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		if (PeaceFulHazardGameMode)
		{
			PeaceFulHazardGameMode->InteractWithItemUIEvent.Broadcast(beforeitemType, beforeitemcount);
			PeaceFulHazardGameMode->PlayUISound(TakeItemSound, 0.3f);
		}
	}
}

bool UInventoryWidget::IsInventoryFull()
{
	if (recentinventory)
	{
		int32 Lockindex = 0;
		for (bool Lock : recentinventory->ItemLockArray)
		{
			if (Lock)
			{
				break;
			}

			Lockindex++;
		}

		int32 Emptyindex = 0;
		for (EItemType InventoryitemType : recentinventory->inventoryItems)
		{
			if (InventoryitemType == EItemType::EIT_None)
			{
				break;
			}

			Emptyindex++;
		}

		return Emptyindex >= Lockindex;
	}

	return true;
}

bool UInventoryWidget::CanGetItem(EItemType itemType, int32 count)
{
	if (!IsInventoryFull()) return true;

	// only if inventory is Full

	int32 MaxCountUnit = ItemInformation->ItemInformationMap[itemType].itemMaxCount;
	int32 FreeSpace = 0;

	if (recentinventory)
	{
		for (int i = 0; i < 15; i++)
		{
			if (recentinventory->inventoryItems[i] != itemType) continue;

			FreeSpace += MaxCountUnit - recentinventory->inventoryItemCounts[i];
		}
	}

	

	return FreeSpace >= count;
}


void UInventoryWidget::SetInventoryDisplay(FCharacterInventoty* inventory)
{
	if (ItemInformation == nullptr) return;

	recentinventory = inventory;

	if (recentinventory == nullptr) return;

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


		if (recentinventory->inventoryItemCounts[i] == 0 || 
			ItemInformation->ItemInformationMap[recentinventory->inventoryItems[i]].itemMaxCount == 1)
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

	SetAllUIUpdate();

}

void UInventoryWidget::showTabUI()
{
	InventoryCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ItemGetCanvas->SetVisibility(ESlateVisibility::Hidden);
	SituationCanvas->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryWidget::showItemGetUI(EItemType itemType, int32 count)
{
	InventoryCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ItemGetCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SituationCanvas->SetVisibility(ESlateVisibility::Hidden);


	if (ItemInformation)
	{
		beforeitemType = itemType;
		beforeitemcount = count;

		SetItemDynamicChangeLanguage(ItemGetItemExplainText, ItemInformation->ItemInformationMap[itemType], false);
		SetItemDynamicChangeLanguage(ItemGetItemNameText, ItemInformation->ItemInformationMap[itemType], true);

		UTexture2D* Texture = ItemInformation->ItemInformationMap[itemType].itemImage;
		if (Texture)
		{
			ItemGetImage->SetBrushFromTexture(Texture);
		}

		if (count == 0 ||
			ItemInformation->ItemInformationMap[itemType].itemMaxCount == 1)
		{
			ItemGetCountBorder->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ItemGetCountBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			FText TempText = FText::AsNumber(count);
			ItemGetCountText->SetText(TempText);

			if (count == ItemInformation->ItemInformationMap[itemType].itemMaxCount)
			{
				ItemGetCountText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
			}
			else
			{
				ItemGetCountText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
			}

		}
	}

	if (!CanGetItem(itemType, count))
	{
		TakeBackground->SetVisibility(ESlateVisibility::Hidden);
		TakeText->SetVisibility(ESlateVisibility::Hidden);
		TakeImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TakeBackground->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TakeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TakeImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

}

void UInventoryWidget::showSituationUI(EInteractSituationType situationType)
{
	InventoryCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ItemGetCanvas->SetVisibility(ESlateVisibility::Hidden);
	SituationCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	situationLock = true;

	if (ItemInformation)
	{
		beforeSituationType = situationType;

		SetSituationDynamicChangeLanguage(SituationExplainText, ItemInformation->SituationMap[situationType], false);
		SetSituationDynamicChangeLanguage(SituationNameText, ItemInformation->SituationMap[situationType], true);


		UTexture2D* Texture = ItemInformation->SituationMap[situationType].situationImage;
		if (Texture)
		{
			SituationImage->SetBrushFromTexture(Texture);
		}

		TArray<EItemType> interactItems = ItemInformation->SituationMap[situationType].InteractItemArray;

		for (UButton* Button : ItemButtons)
		{
			int32 i = GetButtonIndex(Button);
			EItemType IT = recentinventory->inventoryItems[i];

			if (interactItems.Contains(IT))
			{
				Button->SetIsEnabled(true);
			}
			else
			{
				Button->SetIsEnabled(false);
			}

		}
	}
}

void UInventoryWidget::CheckLanguage()
{
	if (PeaceFulHazardGameMode == nullptr) return;

	SetLangaugeText(PeaceFulHazardGameMode->GetCurrentLanguage());

}

void UInventoryWidget::SetItemDynamicChangeLanguage(UTextBlock* textBlock, FItemInformationUnit itemInformtation, bool bName)
{
	if (PeaceFulHazardGameMode == nullptr) return;

	FText Englishtext = FText();
	FText Koreantext = FText();

	switch (PeaceFulHazardGameMode->GetCurrentLanguage())
	{
	case ELanguage::ED_English:

		if (bName)
		{
			Englishtext = FText::FromString(itemInformtation.itemNameText);
		}
		else
		{
			Englishtext = FText::FromString(itemInformtation.itemExplainText);
		}
		
		textBlock->SetText(Englishtext);
		break;

	case ELanguage::ED_Korean:

		if (bName)
		{
			Koreantext = FText::FromString(itemInformtation.itemNameKoreanText);
		}
		else
		{
			Koreantext = FText::FromString(itemInformtation.itemExplainKoreanText);
		}

		textBlock->SetText(Koreantext);
		break;

	}
}

void UInventoryWidget::SetSituationDynamicChangeLanguage(UTextBlock* textBlock, FSituationInformationUnit situation, bool bName)
{
	if (PeaceFulHazardGameMode == nullptr) return;

	FText Englishtext = FText();
	FText Koreantext = FText();

	switch (PeaceFulHazardGameMode->GetCurrentLanguage())
	{
	case ELanguage::ED_English:

		if (bName)
		{
			Englishtext = FText::FromString(situation.situationNameText);
		}
		else
		{
			Englishtext = FText::FromString(situation.situationExplainText);
		}

		textBlock->SetText(Englishtext);
		break;

	case ELanguage::ED_Korean:

		if (bName)
		{
			Koreantext = FText::FromString(situation.situationNameKoreanText);
		}
		else
		{
			Koreantext = FText::FromString(situation.situationExplainKoreanText);
		}

		textBlock->SetText(Koreantext);
		break;

	}
}

void UInventoryWidget::CloseInventoryUIValue()
{
	InteractLock = false;
	combineLock = false;
	MoveLock = false;
	situationLock = false;

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

	if (MoveButton)
	{
		MoveButton->OnClicked.AddDynamic(this, &ThisClass::OnMoveButtoClicked);
	}


	UIInteractCanvas->SetVisibility(ESlateVisibility::Hidden);

}

void UInventoryWidget::OnUseButtoClicked()
{
	if (NowHoveringButton == nullptr) return;
	if (ItemInformation == nullptr) return;

	int32 index = GetButtonIndex(NowHoveringButton);
	EItemType itemType = recentinventory->inventoryItems[index];

	if (PeaceFulHazardGameMode)
	{
		if (situationLock)
		{
			if (beforeSituationType == EInteractSituationType::EIST_None) return;

			situationLock = false;
			PeaceFulHazardGameMode->InteractSituationEvent.Broadcast(beforeSituationType);
			if (!ItemInformation->ItemInformationMap[itemType].bReusable)
			{
				PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(GetButtonIndex(NowHoveringButton), itemType, -1, false);
			}

		}
		else
		{
			PeaceFulHazardGameMode->UseItemEvent.Broadcast(itemType, false);
			PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(GetButtonIndex(NowHoveringButton), itemType, -1, false);

		}
	}

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}


}

void UInventoryWidget::OnCombineButtoClicked()
{
	combineLock = true;
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}

	SetAllUIUpdate();
}

void UInventoryWidget::OnDiscardButtoClicked()
{
	if (NowHoveringButton == nullptr) return;

	int32 index = GetButtonIndex(NowHoveringButton);
	EItemType itemType = recentinventory->inventoryItems[index];
	int32 itemCount = recentinventory->inventoryItemCounts[index];

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(GetButtonIndex(NowHoveringButton), itemType, -1, false);
	}
}

void UInventoryWidget::OnMoveButtoClicked()
{
	MoveLock = true;
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}

	SetAllUIUpdate();

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

bool UInventoryWidget::IsItemisMax(UButton* button)
{
	if (button == nullptr) return true;
	if (ItemInformation == nullptr) return true;

	int32 index = GetButtonIndex(button);

	int32 itemcount = recentinventory->inventoryItemCounts[index];
	int32 maxCount = ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]].itemMaxCount;

	if (maxCount == 1)
	{
		return false;
	}
	else
	{
		return itemcount >= maxCount;
	}
}


void UInventoryWidget::ChangeNowHoveringButton(UButton* button, bool force)
{
	LiteralHoveringButton = button;

	if (InteractLock && !force) return;

	NowHoveringButton = button;

	SetAllUIUpdate();
}

bool UInventoryWidget::CombineItem(UButton* originButton, UButton* addButton)
{
	if (originButton == nullptr) return false;
	if (addButton == nullptr) return false;
	if (addButton == originButton) return false;
	if (ItemInformation == nullptr) return false;


	int32 originIndex = GetButtonIndex(originButton);
	int32 addIndex = GetButtonIndex(addButton);

	EItemType originItemType = recentinventory->inventoryItems[originIndex];
	EItemType addItemType = recentinventory->inventoryItems[addIndex];

	int32 originCount = recentinventory->inventoryItemCounts[originIndex];
	int32 addCount = recentinventory->inventoryItemCounts[addIndex];

	TMap<EItemType, EItemType> originInteractArray = ItemInformation->ItemInformationMap[originItemType].ItemCombineMap;

	if (!originInteractArray.Contains(addItemType)) return false;

	if (originInteractArray[addItemType] == addItemType)
	{
		if (PeaceFulHazardGameMode)
		{
			PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(addIndex, addItemType, -addCount, false);
			PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(originIndex, originItemType, addCount, false);
		}
	}
	else
	{
		if (PeaceFulHazardGameMode)
		{
			PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(addIndex, addItemType, -addCount, false);
			PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(originIndex, originInteractArray[addItemType], originCount, true);
		}
	}

	NowInteractButton = originButton;
	ChangeNowHoveringButton(originButton, true);

	return true;
}

bool UInventoryWidget::MoveItem(UButton* originButton, UButton* addButton)
{
	if (originButton == nullptr) return false;
	if (addButton == nullptr) return false;
	if (addButton == originButton) return false;


	int32 originIndex = GetButtonIndex(originButton);
	int32 addIndex = GetButtonIndex(addButton);

	EItemType addItemType = recentinventory->inventoryItems[addIndex];
	int32 addCount = recentinventory->inventoryItemCounts[addIndex];


	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(addIndex, addItemType, -addCount, false);
		PeaceFulHazardGameMode->OuterChangeInventoryEvent.Broadcast(originIndex, addItemType, addCount, true);
	}

	NowInteractButton = originButton;
	ChangeNowHoveringButton(originButton, true);

	return true;

}

void UInventoryWidget::OnItemButtonClicked()
{
	if (!CanInteractButton(NowHoveringButton)) return;
	if (LiteralHoveringButton == nullptr) return;

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonClickSound, 1.f);
	}

	if (combineLock)
	{
		if (!CombineItem(LiteralHoveringButton, NowInteractButton)) return;

		combineLock = false;
	}
	else if (MoveLock)
	{
		if (!MoveItem(LiteralHoveringButton, NowInteractButton)) return;

		MoveLock = false;
	}
	else
	{
		InteractLock = true;
		NowInteractButton = NowHoveringButton;
	}


	SetAllUIUpdate();
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

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->PlayUISound(ButtonHoverSound, 0.5f);
	}
	ChangeNowHoveringButton(FindButton);
}

void UInventoryWidget::OnItemButtonUnhovered()
{
	ChangeNowHoveringButton(nullptr);
	
}

void UInventoryWidget::SetAllUIUpdate()
{
	CheckLanguage();

	SetInventoryCanvas();
	SetItemExplainText();

	if (combineLock || MoveLock)
	{
		SetCombineUIState();
		SetMoveUIState();
	}
	else if (situationLock)
	{

	}
	else
	{
		if (InventoryBackGround)
		{
			FLinearColor Color = InventoryBackGround->BrushColor;
			Color.A = 0.5f;
			InventoryBackGround->SetBrushColor(Color);
		}

		for (UButton* Button : ItemButtons)
		{
			Button->SetIsEnabled(true);
		}
	}

	
}

void UInventoryWidget::SetCombineUIState()
{
	if (ItemInformation == nullptr) return;

	if (combineLock && NowInteractButton != nullptr)
	{
		if (InventoryBackGround)
		{
			FLinearColor Color = InventoryBackGround->BrushColor;
			Color.A = 1.f;
			InventoryBackGround->SetBrushColor(Color);
		}

		int32 interactIndex = GetButtonIndex(NowInteractButton);
		EItemType itemType = recentinventory->inventoryItems[interactIndex];
		TMap<EItemType, EItemType> interactMap = ItemInformation->ItemInformationMap[itemType].ItemCombineMap;

		for (UButton* Button : ItemButtons)
		{
			int32 i = GetButtonIndex(Button);
			EItemType IT = recentinventory->inventoryItems[i];

			if (interactMap.Contains(IT) && !IsItemisMax(Button))
			{
				Button->SetIsEnabled(true);
			}
			else
			{
				Button->SetIsEnabled(false);
			}

		}
	}
	


}

void UInventoryWidget::SetMoveUIState()
{
	if (MoveLock && NowInteractButton != nullptr)
	{
		if (InventoryBackGround)
		{
			FLinearColor Color = InventoryBackGround->BrushColor;
			Color.A = 1.f;
			InventoryBackGround->SetBrushColor(Color);
		}

		int32 interactIndex = GetButtonIndex(NowInteractButton);
		EItemType itemType = recentinventory->inventoryItems[interactIndex];

		for (UButton* Button : ItemButtons)
		{
			int32 i = GetButtonIndex(Button);

			if (recentinventory->inventoryItems[i] == EItemType::EIT_None)
			{
				Button->SetIsEnabled(true);
			}
			else
			{
				Button->SetIsEnabled(false);
			}

		}
	}
	
}

void UInventoryWidget::SetInventoryCanvas()
{
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
		NowInteractButton = nullptr;
	}

	if (InteractLock)
	{
		if (situationLock)
		{
			CombineModeBorder->SetVisibility(ESlateVisibility::Hidden);
			MoveModeBorder->SetVisibility(ESlateVisibility::Hidden);

			InteractBackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UseButton->SetVisibility(ESlateVisibility::Visible);
			CombineButton->SetVisibility(ESlateVisibility::Hidden);
			DiscardButton->SetVisibility(ESlateVisibility::Hidden);
			MoveButton->SetVisibility(ESlateVisibility::Hidden);

		}
		else
		{
			if (combineLock || MoveLock)
			{
				if (combineLock)
				{
					CombineModeBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					MoveModeBorder->SetVisibility(ESlateVisibility::Hidden);
				}
				else if (MoveLock)
				{
					CombineModeBorder->SetVisibility(ESlateVisibility::Hidden);
					MoveModeBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}

				InteractBackGround->SetVisibility(ESlateVisibility::Hidden);
				UseButton->SetVisibility(ESlateVisibility::Hidden);
				CombineButton->SetVisibility(ESlateVisibility::Hidden);
				DiscardButton->SetVisibility(ESlateVisibility::Hidden);
				MoveButton->SetVisibility(ESlateVisibility::Hidden);

			}
			else
			{
				CombineModeBorder->SetVisibility(ESlateVisibility::Hidden);
				MoveModeBorder->SetVisibility(ESlateVisibility::Hidden);

				InteractBackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				UseButton->SetVisibility(ESlateVisibility::Visible);
				CombineButton->SetVisibility(ESlateVisibility::Visible);
				DiscardButton->SetVisibility(ESlateVisibility::Visible);
				MoveButton->SetVisibility(ESlateVisibility::Visible);
			}
		}

	}
	else
	{
		CombineModeBorder->SetVisibility(ESlateVisibility::Hidden);
		MoveModeBorder->SetVisibility(ESlateVisibility::Hidden);

		InteractBackGround->SetVisibility(ESlateVisibility::Hidden);
		UseButton->SetVisibility(ESlateVisibility::Hidden);
		CombineButton->SetVisibility(ESlateVisibility::Hidden);
		DiscardButton->SetVisibility(ESlateVisibility::Hidden);
		MoveButton->SetVisibility(ESlateVisibility::Hidden);

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
			SetItemDynamicChangeLanguage(ItemExplainText, ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]], false);
			SetItemDynamicChangeLanguage(ItemNameText, ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]], true);
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
			if (ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]].bKeyItem )
			{
				if (situationLock)
				{
					UseButton->SetIsEnabled(true);
				}
				else
				{
					UseButton->SetIsEnabled(false);
				}
			}
			else
			{
				UseButton->SetIsEnabled(true);
			}
			
		}
		else
		{
			UseButton->SetIsEnabled(false);
		}

		if (ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]].ItemCombineMap.Num() > 0)
		{
			if (IsItemisMax(NowHoveringButton))
			{
				CombineButton->SetIsEnabled(false);
			}
			else
			{
				CombineButton->SetIsEnabled(true);
			}
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

