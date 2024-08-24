// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NoticePanelWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h"

void UNoticePanelWidget::BackUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (PeaceFulHazardGameMode)
	{
		FString str = FString("");
		PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(false, str);
	}

}

void UNoticePanelWidget::OkUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (PeaceFulHazardGameMode)
	{
		FString str = FString("");
		PeaceFulHazardGameMode->NoticeUIShowEvent.Broadcast(false, str);
	}
}

void UNoticePanelWidget::UpdateNoticeDisplay(FString& noticeText)
{
	FText TempText = FText::FromString(noticeText);
	NoticeText->SetText(TempText);
}

void UNoticePanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

}
