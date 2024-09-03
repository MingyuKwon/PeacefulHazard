// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemInformation.h"
#include "DefaultPlayerWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UBorder;
class APeaceFulHazardGameMode;
class UPeacFulGameInstance;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API UDefaultPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	UPeacFulGameInstance* PeacFulGameInstance;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* LoadingCanvas;


	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxBulletText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentBulletText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LeftBulletText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LeftAnotherBulletText;

	
	UPROPERTY(meta = (BindWidget))
	UImage* PistolImage;

	UPROPERTY(meta = (BindWidget))
	UImage* PistolBulletImage;

	UPROPERTY(meta = (BindWidget))
	UImage* PistolAnotherBulletImage;

	UPROPERTY(meta = (BindWidget))
	UImage* NowBackGround;

	UPROPERTY(meta = (BindWidget))
	UImage* AnotherBackGround;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TodoText;


	UPROPERTY(meta = (BindWidget))
	UProgressBar* Healthbar;

	float lerpHealth = 1.f;
	float showHealth = 1.f;

	FTimerHandle updateHealthHandle;

public:

	UFUNCTION(BlueprintCallable)
	void UpdateBulletUI(int32 currentBullet, int32 maxBullet, int32 leftBullet, EItemType itemType, int32 anotherBullet);

	UFUNCTION(BlueprintCallable)
	void UpdateTodoUI();

	UFUNCTION(BlueprintCallable)
	void ShowLoadingUI(bool bVisible);

	UFUNCTION(BlueprintCallable)
	void UpdateHealthUI(float health, bool bforce);

};
