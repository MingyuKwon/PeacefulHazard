// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CanInteractWidget.h"
#include "Components/Image.h"

void UCanInteractWidget::SetActionEnable(bool flag)
{
	if (flag)
	{
		ActionImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ActionImage->SetVisibility(ESlateVisibility::Hidden);

	}
}

void UCanInteractWidget::SetInteractEnable(bool flag)
{
	if (flag)
	{
		InteractImage->SetVisibility(ESlateVisibility::Visible);

	}
	else
	{
		InteractImage->SetVisibility(ESlateVisibility::Hidden);

	}

}
