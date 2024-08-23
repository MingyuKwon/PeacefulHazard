// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemBoxWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h"

void UItemBoxWidget::BackUIInputTrigger()
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
	else if (InteractLock)
	{
		InteractLock = false;
		NowInteractButton = nullptr;
	}
	else
	{
		if (PeaceFulHazardGameMode)
		{
			PeaceFulHazardGameMode->CloseAllUIEvent.Broadcast();
		}

		return;
	}

	SetAllUIUpdate();

}

void UItemBoxWidget::OkUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

}

bool UItemBoxWidget::IsInventoryFull()
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


void UItemBoxWidget::SetItemBoxDisplay(FCharacterInventoty* inventory, FCharacterItemBox* itemBox)
{
	recentinventory = inventory;
	recentItemBox = itemBox;

	if (recentinventory == nullptr) return;
	if (recentItemBox == nullptr) return;

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

	for (int i = 0; i < 20; i++)
	{
		FText TempText;

		if (recentItemBox->itemBoxItemCounts[i] == 0 ||
			ItemInformation->ItemInformationMap[recentItemBox->itemBoxItems[i]].itemMaxCount == 1)
		{
			ItemCountBorders_1[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ItemCountBorders_1[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			TempText = FText::AsNumber(recentItemBox->itemBoxItemCounts[i]);
			ItemCountTexts_1[i]->SetText(TempText);

			if (recentItemBox->itemBoxItemCounts[i] == ItemInformation->ItemInformationMap[recentItemBox->itemBoxItems[i]].itemMaxCount)
			{
				ItemCountTexts_1[i]->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
			}
			else
			{
				ItemCountTexts_1[i]->SetColorAndOpacity(FSlateColor(FLinearColor::White));
			}

		}

		if (ItemInformation)
		{
			UTexture2D* Texture = ItemInformation->ItemInformationMap[recentItemBox->itemBoxItems[i]].itemImage;
			if (Texture)
			{
				ItemImages_1[i]->SetBrushFromTexture(Texture);
			}
		}
	}

	SetAllUIUpdate();

}

void UItemBoxWidget::showItemBoxUI()
{
	InventoryCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UItemBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));


	InitArrays();
	InitArrays_1();

	for (UButton* Button : ItemButtons)
	{
		if (Button)
		{
			Button->OnClicked.AddDynamic(this, &ThisClass::OnItemButtonClicked);
			Button->OnHovered.AddDynamic(this, &ThisClass::OnItemButtonHovered);
			Button->OnUnhovered.AddDynamic(this, &ThisClass::OnItemButtonUnhovered);
		}
	}

	for (UButton* Button : ItemButtons_1)
	{
		if (Button)
		{
			Button->OnClicked.AddDynamic(this, &ThisClass::OnItemButtonClicked_1);
			Button->OnHovered.AddDynamic(this, &ThisClass::OnItemButtonHovered_1);
			Button->OnUnhovered.AddDynamic(this, &ThisClass::OnItemButtonUnhovered_1);
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


void UItemBoxWidget::OnUseButtoClicked()
{
	if (NowHoveringButton == nullptr) return;

	int32 index = GetButtonIndex(NowHoveringButton);
	EItemType itemType = recentinventory->inventoryItems[index];
	int32 itemCount = recentinventory->inventoryItemCounts[index];

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->ItemBoxInteractEvent.Broadcast(true, index, itemType, itemCount);
	}

}

void UItemBoxWidget::OnCombineButtoClicked()
{
	combineLock = true;

	SetAllUIUpdate();
}

void UItemBoxWidget::OnDiscardButtoClicked()
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

void UItemBoxWidget::OnMoveButtoClicked()
{
	MoveLock = true;
	SetAllUIUpdate();

}

bool UItemBoxWidget::CanInteractButton_1(UButton* button)
{
	int32 index = GetButtonIndex_1(button);

	if (index >= 0)
	{
		if (recentItemBox->itemBoxItems[index] != EItemType::EIT_None)
		{
			return true;
		}

	}

	return false;
}


bool UItemBoxWidget::CanInteractButton(UButton* button)
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

int32 UItemBoxWidget::GetButtonIndex_1(UButton* button)
{
	int32 index = 0;

	for (UButton* Button : ItemButtons_1)
	{
		if (Button == button)
		{
			break;
		}

		index++;
	}

	return index < 20 ? index : -1;
}

int32 UItemBoxWidget::GetButtonIndex(UButton* button)
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

bool UItemBoxWidget::IsItemisMax(UButton* button)
{
	if (button == nullptr) return true;

	int32 index = GetButtonIndex(button);

	int32 itemcount = recentinventory->inventoryItemCounts[index];
	int32 maxCount = ItemInformation->ItemInformationMap[recentinventory->inventoryItems[index]].itemMaxCount;

	return itemcount >= maxCount;
}


void UItemBoxWidget::ChangeNowHoveringButton(UButton* button, bool force)
{
	LiteralHoveringButton = button;

	if (InteractLock && !force) return;

	NowHoveringButton = button;

	SetAllUIUpdate();
}

bool UItemBoxWidget::CombineItem(UButton* originButton, UButton* addButton)
{
	if (originButton == nullptr) return false;
	if (addButton == nullptr) return false;
	if (addButton == originButton) return false;


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

bool UItemBoxWidget::MoveItem(UButton* originButton, UButton* addButton)
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



void UItemBoxWidget::OnItemButtonClicked_1()
{
	if (LiteralHoveringButton_1 == nullptr) return;
	UE_LOG(LogTemp, Display, TEXT("OnItemButtonClicked_1"));

	if (!CanInteractButton_1(LiteralHoveringButton_1)) return;

	if (PeaceFulHazardGameMode)
	{
		int32 index = GetButtonIndex_1(LiteralHoveringButton_1);
		PeaceFulHazardGameMode->ItemBoxInteractEvent.Broadcast(false, index, recentItemBox->itemBoxItems[index], recentItemBox->itemBoxItemCounts[index]);

	}
	

	SetAllUIUpdate();
}

void UItemBoxWidget::OnItemButtonHovered_1()
{
	UButton* FindButton = nullptr;

	for (UButton* Button : ItemButtons_1)
	{
		if (Button->IsHovered())
		{
			FindButton = Button;
			break;
		}
	}

	LiteralHoveringButton_1 = FindButton;
	SetAllUIUpdate();
}

void UItemBoxWidget::OnItemButtonUnhovered_1()
{
	LiteralHoveringButton_1 = nullptr;
	SetAllUIUpdate();
}


void UItemBoxWidget::OnItemButtonClicked()
{
	if (!CanInteractButton(NowHoveringButton)) return;
	if (LiteralHoveringButton == nullptr) return;

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

void UItemBoxWidget::OnItemButtonHovered()
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

void UItemBoxWidget::OnItemButtonUnhovered()
{
	ChangeNowHoveringButton(nullptr);

}



// 이제 여기서 창고 부분에서 일어난 일도 다 업데이트 시키도록 업뎅트 ㅎ마수들 싹 창고 버전은로 바꿔야 함
// 그리고 move나 combine, interact lock이 결려 있는 경우에 , 창고는 싹다 애들 버튼 disable 시켜야 함
// 창고는 애들 자동으로 앞에서 부터 채워지도록 만들까?
void UItemBoxWidget::SetAllUIUpdate()
{
	SetInventoryCanvas();
	SetInventoryCanvas_1();

	SetItemExplainText();
	SetItemExplainText_1();

	if (combineLock || MoveLock)
	{
		SetCombineUIState();
		SetMoveUIState();

		for (UButton* Button : ItemButtons_1)
		{
			Button->SetIsEnabled(false);
		}
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

		if (InteractLock)
		{
			for (UButton* Button : ItemButtons_1)
			{
				Button->SetIsEnabled(false);
			}
		}
		else
		{
			for (UButton* Button : ItemButtons_1)
			{
				Button->SetIsEnabled(true);
			}
		}
	}


}

void UItemBoxWidget::SetCombineUIState()
{
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

void UItemBoxWidget::SetMoveUIState()
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

void UItemBoxWidget::SetInventoryCanvas_1()
{
	if (LiteralHoveringButton_1 == nullptr)
	{
		UIBoxInteractCanvas->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	UIBoxInteractCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (UCanvasPanelSlot* ButtonSlot = Cast<UCanvasPanelSlot>(LiteralHoveringButton_1->Slot))
	{
		FVector2D ButtonPosition = ButtonSlot->GetPosition();

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(UIBoxInteractCanvas->Slot))
		{
			CanvasSlot->SetPosition(ButtonPosition);
		}
	}
}

void UItemBoxWidget::SetInventoryCanvas()
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

void UItemBoxWidget::SetItemExplainText_1()
{
	FText TempText = FText();

	if (LiteralHoveringButton_1 == nullptr)
	{
		if (NowHoveringButton == nullptr)
		{
			ItemNameText->SetText(TempText);
			ItemExplainText->SetText(TempText);
		}

	}
	else
	{
		int32 index = GetButtonIndex_1(LiteralHoveringButton_1);


		if (ItemInformation)
		{
			FString string = ItemInformation->ItemInformationMap[recentItemBox->itemBoxItems[index]].itemExplainText;
			TempText = FText::FromString(string);
			ItemExplainText->SetText(TempText);

			string = ItemInformation->ItemInformationMap[recentItemBox->itemBoxItems[index]].itemNameText;
			TempText = FText::FromString(string);
			ItemNameText->SetText(TempText);

		}


	}
}



void UItemBoxWidget::SetItemExplainText()
{
	FText TempText = FText();

	if (NowHoveringButton == nullptr)
	{
		if (LiteralHoveringButton_1 == nullptr)
		{
			ItemNameText->SetText(TempText);
			ItemExplainText->SetText(TempText);
		}

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

void UItemBoxWidget::SetInteractPanelButton()
{
	if (NowHoveringButton == nullptr) return;

	int32 index = GetButtonIndex(NowHoveringButton);

	if (ItemInformation)
	{
		UseButton->SetIsEnabled(true);

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

void UItemBoxWidget::InitArrays_1()
{
	ItemButtons_1.Add(ItemButton1_1);
	ItemButtons_1.Add(ItemButton2_1);
	ItemButtons_1.Add(ItemButton3_1);
	ItemButtons_1.Add(ItemButton4_1);
	ItemButtons_1.Add(ItemButton5_1);
	ItemButtons_1.Add(ItemButton6_1);
	ItemButtons_1.Add(ItemButton7_1);
	ItemButtons_1.Add(ItemButton8_1);
	ItemButtons_1.Add(ItemButton9_1);
	ItemButtons_1.Add(ItemButton10_1);
	ItemButtons_1.Add(ItemButton11_1);
	ItemButtons_1.Add(ItemButton12_1);
	ItemButtons_1.Add(ItemButton13_1);
	ItemButtons_1.Add(ItemButton14_1);
	ItemButtons_1.Add(ItemButton15_1);
	ItemButtons_1.Add(ItemButton16_1);
	ItemButtons_1.Add(ItemButton17_1);
	ItemButtons_1.Add(ItemButton18_1);
	ItemButtons_1.Add(ItemButton19_1);
	ItemButtons_1.Add(ItemButton20_1);


	ItemImages_1.Add(ItemImage1_1);
	ItemImages_1.Add(ItemImage2_1);
	ItemImages_1.Add(ItemImage3_1);
	ItemImages_1.Add(ItemImage4_1);
	ItemImages_1.Add(ItemImage5_1);
	ItemImages_1.Add(ItemImage6_1);
	ItemImages_1.Add(ItemImage7_1);
	ItemImages_1.Add(ItemImage8_1);
	ItemImages_1.Add(ItemImage9_1);
	ItemImages_1.Add(ItemImage10_1);
	ItemImages_1.Add(ItemImage11_1);
	ItemImages_1.Add(ItemImage12_1);
	ItemImages_1.Add(ItemImage13_1);
	ItemImages_1.Add(ItemImage14_1);
	ItemImages_1.Add(ItemImage15_1);
	ItemImages_1.Add(ItemImage16_1);
	ItemImages_1.Add(ItemImage17_1);
	ItemImages_1.Add(ItemImage18_1);
	ItemImages_1.Add(ItemImage19_1);
	ItemImages_1.Add(ItemImage20_1);


	ItemCountTexts_1.Add(ItemCountText1_1);
	ItemCountTexts_1.Add(ItemCountText2_1);
	ItemCountTexts_1.Add(ItemCountText3_1);
	ItemCountTexts_1.Add(ItemCountText4_1);
	ItemCountTexts_1.Add(ItemCountText5_1);
	ItemCountTexts_1.Add(ItemCountText6_1);
	ItemCountTexts_1.Add(ItemCountText7_1);
	ItemCountTexts_1.Add(ItemCountText8_1);
	ItemCountTexts_1.Add(ItemCountText9_1);
	ItemCountTexts_1.Add(ItemCountText10_1);
	ItemCountTexts_1.Add(ItemCountText11_1);
	ItemCountTexts_1.Add(ItemCountText12_1);
	ItemCountTexts_1.Add(ItemCountText13_1);
	ItemCountTexts_1.Add(ItemCountText14_1);
	ItemCountTexts_1.Add(ItemCountText15_1);
	ItemCountTexts_1.Add(ItemCountText16_1);
	ItemCountTexts_1.Add(ItemCountText17_1);
	ItemCountTexts_1.Add(ItemCountText18_1);
	ItemCountTexts_1.Add(ItemCountText19_1);
	ItemCountTexts_1.Add(ItemCountText20_1);


	ItemCountBorders_1.Add(ItemCountBorder1_1);
	ItemCountBorders_1.Add(ItemCountBorder2_1);
	ItemCountBorders_1.Add(ItemCountBorder3_1);
	ItemCountBorders_1.Add(ItemCountBorder4_1);
	ItemCountBorders_1.Add(ItemCountBorder5_1);
	ItemCountBorders_1.Add(ItemCountBorder6_1);
	ItemCountBorders_1.Add(ItemCountBorder7_1);
	ItemCountBorders_1.Add(ItemCountBorder8_1);
	ItemCountBorders_1.Add(ItemCountBorder9_1);
	ItemCountBorders_1.Add(ItemCountBorder10_1);
	ItemCountBorders_1.Add(ItemCountBorder11_1);
	ItemCountBorders_1.Add(ItemCountBorder12_1);
	ItemCountBorders_1.Add(ItemCountBorder13_1);
	ItemCountBorders_1.Add(ItemCountBorder14_1);
	ItemCountBorders_1.Add(ItemCountBorder15_1);
	ItemCountBorders_1.Add(ItemCountBorder16_1);
	ItemCountBorders_1.Add(ItemCountBorder17_1);
	ItemCountBorders_1.Add(ItemCountBorder18_1);
	ItemCountBorders_1.Add(ItemCountBorder19_1);
	ItemCountBorders_1.Add(ItemCountBorder20_1);

}

void UItemBoxWidget::InitArrays()
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
