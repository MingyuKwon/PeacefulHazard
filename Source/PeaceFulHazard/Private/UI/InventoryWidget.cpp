// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemButton1)
	{
		ItemButton1->OnClicked.AddDynamic(this, &UInventoryWidget::OnItemButtonClicked);
		ItemButton1->OnHovered.AddDynamic(this, &UInventoryWidget::OnItemButtonHovered);
		ItemButton1->OnUnhovered.AddDynamic(this, &UInventoryWidget::OnItemButtonUnhovered);
	}
}

void UInventoryWidget::OnItemButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemButton1 clicked!"));
}

void UInventoryWidget::OnItemButtonHovered()
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse hovered over ItemButton1!"));
}

void UInventoryWidget::OnItemButtonUnhovered()
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse unhovered from ItemButton1!"));
}

