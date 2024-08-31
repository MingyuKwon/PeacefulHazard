// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InformationPanelWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class APeaceFulHazardGameMode;

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UInformationPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BackUIInputTrigger();

	void OkUIInputTrigger();

	void UpdateNoticeDisplay(FString& noticeText);

protected:

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NoticeText;

	UPROPERTY(meta = (BindWidget))
	UBorder* CloseBackGround;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CloseText;

	UPROPERTY(meta = (BindWidget))
	UBorder* RightButtonImage;

	UPROPERTY(meta = (BindWidget))
	UBorder* LeftButtonImage;


};
