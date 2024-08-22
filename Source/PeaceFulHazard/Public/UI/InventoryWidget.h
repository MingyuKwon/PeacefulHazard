#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Controller/HappyPlayerController.h"
#include "Item/ItemInformation.h"
#include "InventoryWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class APeaceFulHazardGameMode;

UCLASS()
class PEACEFULHAZARD_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BackUIInputTrigger();

	void SetInventoryDisplay(FCharacterInventoty* inventory);

	bool InteractLock = false;

	bool combineLock = false;

	AItemInformation* ItemInformation;

protected:

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	virtual void NativeConstruct() override;

	bool CanInteractButton(UButton* button);

	int32 GetButtonIndex(UButton* button);

	void SetCombineUIState();

	void SetAllUIUpdate ();

	void SetInventoryCanvas();

	void SetItemExplainText();

	void SetInteractPanelButton();


	FCharacterInventoty* recentinventory;


	UButton* NowHoveringButton = nullptr;

	UButton* NowInteractButton = nullptr;

	void ChangeNowHoveringButton(UButton* button);

	UFUNCTION()
	void OnItemButtonClicked();

	UFUNCTION()
	void OnItemButtonHovered();

	UFUNCTION()
	void OnItemButtonUnhovered();



	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* UIInteractCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* InventoryCanvas;

	TArray<UButton*> ItemButtons;

	TArray<UImage*> ItemImages;

	TArray<UTextBlock*> ItemCountTexts;

	TArray<UBorder*> ItemCountBorders;

	//////////////////////////////////////// Button Start//////////////////////////////////////////////////////////


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
	UImage* InteractBackGround;

	UPROPERTY(meta = (BindWidget))
	UBorder* CombineModeBorder;


	UFUNCTION()
	void OnUseButtoClicked();

	UFUNCTION()
	void OnCombineButtoClicked();

	UFUNCTION()
	void OnDiscardButtoClicked();



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


	//////////////////////////////////////// Button End //////////////////////////////////////////////////////////

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