// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Controller/HappyPlayerController.h"
#include "Item/ItemInformation.h"
#include "ItemBoxWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class APeaceFulHazardGameMode;
class USoundBase;

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UItemBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BackUIInputTrigger();

	void OkUIInputTrigger();

	bool IsInventoryFull();

	bool IsBoxFull();

	void SetItemBoxDisplay(FCharacterInventoty* inventory, FCharacterItemBox* itemBox);

	void showItemBoxUI();

	bool InteractLock = false;
	bool combineLock = false;
	bool MoveLock = false;

	AItemInformation* ItemInformation;

protected:

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonHoverSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* ButtonClickSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* BackSound;


	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* UIInteractCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* UIBoxInteractCanvas;


	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* InventoryCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* ItemBoxCanvas;


	///////////////////////////////////////// Item Box Canvas /////////////////////////////////////////////////////////

	UButton* LiteralHoveringButton_1 = nullptr;

	bool CanInteractButton_1(UButton* button);
	int32 GetButtonIndex_1(UButton* button);

	UFUNCTION()
	void OnItemButtonClicked_1();

	UFUNCTION()
	void OnItemButtonHovered_1();

	UFUNCTION()
	void OnItemButtonUnhovered_1();

	UFUNCTION()
	void SetItemExplainText_1();

	void SetInventoryCanvas_1();


	TArray<UButton*> ItemButtons_1;

	TArray<UImage*> ItemImages_1;

	TArray<UTextBlock*> ItemCountTexts_1;

	TArray<UBorder*> ItemCountBorders_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton1_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton2_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton3_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton4_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton5_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton6_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton7_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton8_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton9_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton10_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton11_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton12_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton13_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton14_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton15_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton16_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton17_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton18_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton19_1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton20_1;



	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage1_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage2_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage3_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage4_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage5_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage6_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage7_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage8_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage9_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage10_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage11_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage12_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage13_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage14_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage15_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage16_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage17_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage18_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage19_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage20_1;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText1_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText2_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText3_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText4_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText5_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText6_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText7_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText8_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText9_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText10_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText11_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText12_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText13_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText14_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText15_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText16_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText17_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText18_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText19_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText20_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BoxInventoryState;

	

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder1_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder2_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder3_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder4_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder5_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder6_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder7_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder8_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder9_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder10_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder11_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder12_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder13_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder14_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder15_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder16_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder17_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder18_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder19_1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder20_1;


	void InitArrays_1();
	

	//////////////////////////////////////// Inventory Canvas//////////////////////////////////////////////////////////

	bool CanInteractButton(UButton* button);

	int32 GetButtonIndex(UButton* button);

	bool IsItemisMax(UButton* button);


	void SetCombineUIState();

	void SetMoveUIState();


	void SetAllUIUpdate();

	void SetInventoryCanvas();

	void SetItemExplainText();

	void SetInteractPanelButton();




	bool CombineItem(UButton* originButton, UButton* addButton);
	bool MoveItem(UButton* originButton, UButton* addButton);


	FCharacterInventoty* recentinventory;
	FCharacterItemBox* recentItemBox;


	UButton* NowHoveringButton = nullptr;

	UButton* NowInteractButton = nullptr;

	UButton* LiteralHoveringButton = nullptr;

	void ChangeNowHoveringButton(UButton* button, bool force = false);

	UFUNCTION()
	void OnItemButtonClicked();

	UFUNCTION()
	void OnItemButtonHovered();

	UFUNCTION()
	void OnItemButtonUnhovered();


	TArray<UButton*> ItemButtons;

	TArray<UImage*> ItemImages;

	TArray<UTextBlock*> ItemCountTexts;

	TArray<UBorder*> ItemCountBorders;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton1;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton2;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton3;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton4;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton5;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton6;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton7;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton8;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton9;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton10;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton11;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton12;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton13;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton14;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton15;




	UPROPERTY(meta = (BindWidget))
	UButton* UseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CombineButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DiscardButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MoveButton;


	UPROPERTY(meta = (BindWidget))
	UImage* InteractBackGround;

	UPROPERTY(meta = (BindWidget))
	UBorder* CombineModeBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* MoveModeBorder;


	UFUNCTION()
	void OnUseButtoClicked();

	UFUNCTION()
	void OnCombineButtoClicked();

	UFUNCTION()
	void OnDiscardButtoClicked();

	UFUNCTION()
	void OnMoveButtoClicked();


	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage1;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage2;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage3;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage4;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage5;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage6;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage7;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage8;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage9;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage10;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage11;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage12;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage13;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage14;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage15;

	UPROPERTY(meta = (BindWidget))
	UBorder* InventoryBackGround;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText3;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText4;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText5;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText6;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText7;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText8;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText9;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText10;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText11;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText12;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText13;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText14;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText15;



	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder1;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder2;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder3;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder4;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder5;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder6;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder7;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder8;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder9;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder10;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder11;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder12;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder13;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder14;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemCountBorder15;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemExplainText;


	void InitArrays();

};
