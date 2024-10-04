// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CanInteractWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCanInteractWidget::SetActionEnable(bool bCanShowUI, bool flag)
{
	if (!bCanShowUI)
	{
		ActionImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (flag)
	{
		ActionImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ActionImage->SetVisibility(ESlateVisibility::Hidden);

	}
}

void UCanInteractWidget::SetInteractEnable(bool bCanShowUI, bool flag, UTexture2D* DoorInteractkeyImage, FString DoorInteractkeyName)
{

	if (!bCanShowUI)
	{
		InteractImage->SetVisibility(ESlateVisibility::Hidden);
		KeyImage->SetVisibility(ESlateVisibility::Hidden);
		KeyNameText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (flag)
	{
		InteractImage->SetVisibility(ESlateVisibility::Visible);

		if (DoorInteractkeyImage != nullptr)
		{
			KeyImage->SetVisibility(ESlateVisibility::Visible);
			KeyImage->SetBrushFromTexture(DoorInteractkeyImage);

		}
		else
		{
			KeyImage->SetVisibility(ESlateVisibility::Hidden);
		}


		KeyNameText->SetVisibility(ESlateVisibility::Visible);
		KeyNameText->SetText(FText::FromString(DoorInteractkeyName));
	}
	else
	{
		InteractImage->SetVisibility(ESlateVisibility::Hidden);
		KeyImage->SetVisibility(ESlateVisibility::Hidden);
		KeyNameText->SetVisibility(ESlateVisibility::Hidden);

	}

}
