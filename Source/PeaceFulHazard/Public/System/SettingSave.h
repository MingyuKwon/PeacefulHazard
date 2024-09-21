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
	float MouseAimSensitivity = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Parameter")
	float SFXVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Parameter")
	float BGMVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Parameter")
	float UIVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	float Brightness = 14.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	FString Resolution = FString("1920 x 1080");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting Parameter", meta = (AllowPrivateAccess = "true"))
	FString Language = FString("English");

	
};
