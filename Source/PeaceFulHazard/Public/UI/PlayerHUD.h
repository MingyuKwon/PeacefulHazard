// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Controller/HappyPlayerController.h"
#include "Item/ItemInformation.h"
#include "PlayerHUD.generated.h"

class UAimCrossHairWidget;
class UDefaultPlayerWidget;
class UInventoryWidget;
class UItemBoxWidget;
class UNoticePanelWidget;
class USaveWidget;
/**
 *
 */
UCLASS()
class PEACEFULHAZARD_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	void SetAimDisplay(bool bVisible);

	void SetInventoryDisplay(bool bVisible);

	void SetItemBoxDisplay(bool bVisible);

	void SetNoticeDisplay(bool bVisible);

	void SetMainMenuDisplay(bool bVisible, bool bSavePanelSave);


	void SetGetItemDisplay(bool bVisible, EItemType itemType = EItemType::EIT_None, int32 count = 0);

	void showSituationUI(bool bVisible, EInteractSituationType situationType = EInteractSituationType::EIST_None);

	void UpdateBulletDisplay(int32 currentBullet, int32 maxBullet, int32 leftBullet, EItemType itemType, int32 anotherBullet);

	void UpdateTodoUI();

	void UpdateInventoryDisplay(FCharacterInventoty* inventory);

	void UpdateItemBoxDisplay(FCharacterInventoty* inventory, FCharacterItemBox* itemBox);

	void UpdateNoticeDisplay(FString& noticeText);


	


	void BackUIInputTrigger();

	void OkUIInputTrigger();

	bool GetCanCloseTab();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAimCrossHairWidget> AimCrossHairWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDefaultPlayerWidget> DefaultPlayerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemBoxWidget> ItemBoxWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UNoticePanelWidget> NoticePanelWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USaveWidget> SaveWidgetClass;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Parameter")
	TSubclassOf<AItemInformation> ItemInformationClass;

	void SetItemInformationToDisplay();


	UAimCrossHairWidget* HUDAimWidget;

	UDefaultPlayerWidget* DefaultPlayerWidget;

	UInventoryWidget* InventoryWidget;

	UItemBoxWidget* ItemBoxWidget;

	UNoticePanelWidget* NoticePanelWidget;

	USaveWidget* SaveWidget;

	int32 beforeCurrentBulle = -1;
	int32 beforeMaxBullet = -1;
	int32 beforeLeftBullet = -1;
	int32 beforeanotherBullet = -1;

	EItemType beforeBulletType = EItemType::EIT_Bullet_Noraml;

	FCharacterInventoty* beforeInventory = nullptr;
	FCharacterItemBox* beforeItemBox = nullptr;

};
