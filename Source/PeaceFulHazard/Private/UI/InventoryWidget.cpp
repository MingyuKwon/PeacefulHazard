// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

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

void UInventoryWidget::OnItemButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Pressed "));

}

void UInventoryWidget::OnItemButtonHovered()
{
	UE_LOG(LogTemp, Display, TEXT("OnItemButtonHovered"));

	bool bFind = false;

	for (UButton* Button : ItemButtons)
	{
		if (Button->IsHovered())
		{
			NowHoveringButton = Button;
			bFind = true;
			break;
		}
	}

	NowHoveringButton = bFind ? NowHoveringButton : nullptr;

	SetInventoryCanvas();
}

void UInventoryWidget::OnItemButtonUnhovered()
{
	NowHoveringButton = nullptr;
	UE_LOG(LogTemp, Display, TEXT("OnItemButtonUnhovered"));
	SetInventoryCanvas();
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
}

