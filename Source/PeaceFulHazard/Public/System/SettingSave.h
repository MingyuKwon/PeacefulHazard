// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingSave.generated.h"

/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API USettingSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseSensitivity = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseAimSensitivity = 0.2f;

};
