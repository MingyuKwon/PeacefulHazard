// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InformationPanelWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h"

void UInformationPanelWidget::BackUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (PeaceFulHazardGameMode)
	{
		FString str = FString("");
		PeaceFulHazardGameMode->InformationUIShowEvent.Broadcast(false, str);

		UE_LOG(LogTemp, Display, TEXT("BackUIInputTrigger in Information"));
	}


}

void UInformationPanelWidget::OkUIInputTrigger()
{
	if (GetVisibility() == ESlateVisibility::Hidden) return;

	if (PeaceFulHazardGameMode)
	{
		FString str = FString("");
		PeaceFulHazardGameMode->InformationUIShowEvent.Broadcast(false, str);
	}


}

void UInformationPanelWidget::UpdateNoticeDisplay(FString& noticeText)
{
	FText TempText = FText::FromString(noticeText);
	NoticeText->SetText(TempText);
}

void UInformationPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

}
