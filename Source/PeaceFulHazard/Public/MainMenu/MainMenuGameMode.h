// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "MainMenuGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMenuModeChangeEvent, EMainMenuType, menuType, bool, bChangeStart);

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FMenuModeChangeEvent MenuModeChangeEvent;

protected:
	virtual void BeginPlay() override;

};
