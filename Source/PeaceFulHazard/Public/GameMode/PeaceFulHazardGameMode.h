// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PeaceFulHazardGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNowAimingEvent, bool, flag);


UCLASS(minimalapi)
class APeaceFulHazardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APeaceFulHazardGameMode();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FNowAimingEvent NowAimingEvent;

};



