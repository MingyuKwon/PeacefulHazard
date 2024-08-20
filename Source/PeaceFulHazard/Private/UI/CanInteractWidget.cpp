// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CanInteractWidget.h"
#include "Components/Image.h"

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

void UCanInteractWidget::SetInteractEnable(bool bCanShowUI, bool flag)
{
	if (!bCanShowUI)
	{
		InteractImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (flag)
	{
		InteractImage->SetVisibility(ESlateVisibility::Visible);

	}
	else
	{
		InteractImage->SetVisibility(ESlateVisibility::Hidden);

	}

}
