// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "SaveWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class APeaceFulHazardGameMode;
class UPeacFulGameInstance;
class USoundBase;
class USlider;
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

	UFUNCTION(BlueprintCallable)
	void UpdateAllUI();

	void SetMainMenuDisplay(EMenuType menuType, bool bSavePanelSave = false);

protected:

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonHoverSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonClickSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* BackSound;


	int32 GetButtonIndex(UButton* button, bool bSaveButtons);

	void ChangeNowHoveringButton(UButton* button, bool bSaveButtons);

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
	UCanvasPanel* MenuSelectCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SaveCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MapCanvas;

	
	UButton* HoveringSaveButton;
	UButton* HoveringDeleteButton;

	TArray<UButton*> SaveButtons;
	TArray<UButton*> DeleteButtons;


	//////////////////////// menu select ///////////////////////////////////////////////

	UFUNCTION()
	void OnMapButtonClicked();
	UFUNCTION()
	void OnSaveLoadButtonClicked();
	UFUNCTION()
	void OnOptionButtonClicked();
	UFUNCTION()
	void OnExitButtonClicked();


	UPROPERTY(meta = (BindWidget))
	UButton* MapButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveLoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* TitleButton;

	//////////////////////// menu select ///////////////////////////////////////////////


	/////////////////////// option panel ////////////////////////////////////////////
	UPROPERTY(meta = (BindWidget))
	USlider* DefaultMouseSensibility;

	UPROPERTY(meta = (BindWidget))
	UBorder* DefaultMouseSensibilityFill;

	float maxDefaultMouseSensibilityFill = 1000;

	UPROPERTY(meta = (BindWidget))
	USlider* AimMouseSensibility;

	UPROPERTY(meta = (BindWidget))
	UBorder* AimMouseSensibilityFill;

	float maxAimMouseSensibilityFill = 1000;

	UFUNCTION()
	void OnDefaultMouseSensibilityValueChanged(float Value);

	UFUNCTION()
	void OnAimMouseSensibilityValueChanged(float Value);

	/////////////////////// option panel ////////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveShowPanel;




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
	UPeacFulGameInstance* PeacFulGameInstance;

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



	TArray<UTextBlock*> ToDoTexts;

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


	TArray<UTextBlock*> DifficultyTexts;

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
	UTextBlock* MapDifficulty;

	

	TArray<UTextBlock*> TimeTexts;

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


	TArray<UTextBlock*> SaveSpotTexts;

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


	

	TArray<UBorder*> SaveSlotBackgrounds;

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


	////////////////// MiniMap //////////////////////////////////

	UPROPERTY(meta = (BindWidget))
	UBorder* MainHubBorder;


	UPROPERTY(meta = (BindWidget))
	UBorder* GraveyardBorder;


	UPROPERTY(meta = (BindWidget))
	UBorder* CathedralBorder;
	UPROPERTY(meta = (BindWidget))
	UBorder* CathedralBorder2;
	UPROPERTY(meta = (BindWidget))
	UBorder* CathedralBorder3;
	UPROPERTY(meta = (BindWidget))
	UBorder* CathedralBorder4;


	UPROPERTY(meta = (BindWidget))
	UBorder* TutorialBorder;
	UPROPERTY(meta = (BindWidget))
	UBorder* TutorialBorder2;
	UPROPERTY(meta = (BindWidget))
	UBorder* TutorialBorder3;
	UPROPERTY(meta = (BindWidget))
	UBorder* TutorialBorder4;
	UPROPERTY(meta = (BindWidget))
	UBorder* TutorialBorder5;


	UPROPERTY(meta = (BindWidget))
	UBorder* CrossOverBorder;
	UPROPERTY(meta = (BindWidget))
	UBorder* CrossOverBorder2;
	UPROPERTY(meta = (BindWidget))
	UBorder* CrossOverBorder3;
	UPROPERTY(meta = (BindWidget))
	UBorder* CrossOverBorder4;
	UPROPERTY(meta = (BindWidget))
	UBorder* CrossOverBorder5;



	UPROPERTY(meta = (BindWidget))
	UBorder* LeftGardenBorder;

	
	UPROPERTY(meta = (BindWidget))
	UBorder* RightGardenBorder;

	void ShowCurrentLocation();
	

	////////////////// MiniMap //////////////////////////////////

	void InitArrays();


};
