// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UAimCrossHairWidget;
class UDefaultPlayerWidget;
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

	void SetBulletDisplay(int32 currentBullet, int32 maxBullet, int32 leftBullet);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAimCrossHairWidget> AimCrossHairWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDefaultPlayerWidget> DefaultPlayerWidgetClass;


	UAimCrossHairWidget* HUDAimWidget;

	UDefaultPlayerWidget* DefaultPlayerWidget;


	int32 beforeCurrentBulle = -1;
	int32 beforeMaxBullet = -1;
	int32 beforeLeftBullet = -1;

};
