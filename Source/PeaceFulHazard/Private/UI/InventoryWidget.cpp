// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UInventoryWidget::BackUIInputTrigger()
{
	InteractLock = false;

	SetInventoryCanvas();
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

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

	for (UButton* Button : ItemButtons)
	{
		if (Button)
		{
			Button->OnClicked.AddDynamic(this, &ThisClass::OnItemButtonClicked);
			Button->OnHovered.AddDynamic(this, &ThisClass::OnItemButtonHovered);
			Button->OnUnhovered.AddDynamic(this, &ThisClass::OnItemButtonUnhovered);
		}
	}

	UIInteractCanvas->SetVisibility(ESlateVisibility::Hidden);

}

void UInventoryWidget::ChangeNowHoveringButton(UButton* button)
{
	if (InteractLock) return;

	NowHoveringButton = button;

	SetInventoryCanvas();
}

void UInventoryWidget::OnItemButtonClicked()
{
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


	if (InteractLock)
	{
		InteractBackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UseButton->SetVisibility(ESlateVisibility::Visible);
		CombineButton->SetVisibility(ESlateVisibility::Visible);
		DiscardButton->SetVisibility(ESlateVisibility::Visible);

	}
	else
	{
		InteractBackGround->SetVisibility(ESlateVisibility::Hidden);
		UseButton->SetVisibility(ESlateVisibility::Hidden);
		CombineButton->SetVisibility(ESlateVisibility::Hidden);
		DiscardButton->SetVisibility(ESlateVisibility::Hidden);

	}
}

