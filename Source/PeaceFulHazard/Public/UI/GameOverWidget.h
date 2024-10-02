// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PeaceFulHazard/PeaceFulHazard.h"

#include "GameOverWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class APeaceFulHazardGameMode;
class UPeacFulGameInstance;
class USoundBase;
class USlider;
class UComboBoxString;

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BackUIInputTrigger();

protected:

	int32 GetButtonIndex(UButton* button, bool bSaveButtons);

	void ChangeNowHoveringButton(UButton* button, bool bSaveButtons);

	UFUNCTION()
	void OnSaveButtonClicked();
	UFUNCTION()
	void OnSaveButtonHovered();
	UFUNCTION()
	void OnSaveButtonUnhovered();

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SaveCanvas;

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* GoTitleButton;



	UFUNCTION()
	void OnContinueButtonClicked();

	UFUNCTION()
	void OnContinueButtonHoverd();

	UFUNCTION()
	void OnLoadButtonClicked();

	UFUNCTION()
	void OnLoadButtonHoverd();

	UFUNCTION()
	void OnGoTitleButtonClicked();

	UFUNCTION()
	void OnGoTitleButtonHoverd();




	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonHoverSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonClickSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* BackSound;



	APeaceFulHazardGameMode* PeaceFulHazardGameMode;
	UPeacFulGameInstance* PeacFulGameInstance;

	UFUNCTION(BlueprintCallable)
	void UpdateAllUI();

	UFUNCTION(BlueprintImplementableEvent)
	void SetLangaugeText(ELanguage language);


	UFUNCTION()
	void CheckLanguage();

	void SetDynamicChangeLanguage(UTextBlock* textBlock, const FText& Englishtext, const FText& Koreantext);

	// Load
	void InitArrays();

	TArray<UButton*> SaveButtons;
	TArray<UTextBlock*> ToDoTexts;
	TArray<UTextBlock*> DifficultyTexts;
	TArray<UTextBlock*> TimeTexts;
	TArray<UTextBlock*> SaveSpotTexts;
	TArray<UBorder*> SaveSlotBackgrounds;

	UButton* HoveringSaveButton;


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


	UPROPERTY(meta = (BindWidget))
	UTextBlock* ToDoText1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ToDoText2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ToDoText3;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ToDoText4;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ToDoText5;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ToDoText6;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ToDoText7;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ToDoText8;



	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyText1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyText2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyText3;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyText4;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyText5;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyText6;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyText7;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DifficultyText8;




	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText3;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText4;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText5;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText6;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText7;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText8;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveSpotText1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveSpotText2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveSpotText3;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveSpotText4;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveSpotText5;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveSpotText6;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveSpotText7;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveSpotText8;


	UPROPERTY(meta = (BindWidget))
	UBorder* SaveSlotBackground1;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveSlotBackground2;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveSlotBackground3;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveSlotBackground4;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveSlotBackground5;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveSlotBackground6;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveSlotBackground7;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveSlotBackground8;


};
