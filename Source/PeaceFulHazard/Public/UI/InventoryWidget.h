#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class PEACEFULHAZARD_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

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

	UFUNCTION()
	void OnItemButtonClicked();

	UFUNCTION()
	void OnItemButtonHovered();

	UFUNCTION()
	void OnItemButtonUnhovered();

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



};