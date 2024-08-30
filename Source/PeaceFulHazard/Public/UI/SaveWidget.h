// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveWidget.generated.h"

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
class PEACEFULHAZARD_API USaveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BackUIInputTrigger();

	void OkUIInputTrigger();

	bool bSaveMode = true;

	void UpdateAllUI();

protected:


	UFUNCTION()
	void OnSaveButtonClicked();
	UFUNCTION()
	void OnSaveButtonHovered();
	UFUNCTION()
	void OnSaveButtonUnhovered();

	UFUNCTION()
	void OnDeleteButtonClicked();
	UFUNCTION()
	void OnDeleteButtonHovered();
	UFUNCTION()
	void OnDeleteButtonUnhovered();

	UFUNCTION()
	void OnNewButtonClicked();

	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* UISaveInteractCanvas;

	TArray<UButton*> SaveButtons;
	TArray<UButton*> DeleteButtons;

	UPROPERTY(meta = (BindWidget))
	UButton* NewSaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton1;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton2;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton3;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton4;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton5;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton6;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton7;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton8;

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	UPROPERTY(meta = (BindWidget))
	UButton* deleteButton1;

	UPROPERTY(meta = (BindWidget))
	UButton* deleteButton2;

	UPROPERTY(meta = (BindWidget))
	UButton* deleteButton3;

	UPROPERTY(meta = (BindWidget))
	UButton* deleteButton4;

	UPROPERTY(meta = (BindWidget))
	UButton* deleteButton5;

	UPROPERTY(meta = (BindWidget))
	UButton* deleteButton6;

	UPROPERTY(meta = (BindWidget))
	UButton* deleteButton7;

	UPROPERTY(meta = (BindWidget))
	UButton* deleteButton8;


	UPROPERTY(meta = (BindWidget))
	UBorder* SaveModeBackGround;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveModeText;

	void InitArrays();


};
