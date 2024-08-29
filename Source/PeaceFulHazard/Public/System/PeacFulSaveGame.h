// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "Controller/HappyPlayerController.h"
#include "PeacFulSaveGame.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnemySave
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Parameter")
    FString enemyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Parameter")
    float enemyHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Parameter")
    FVector enemyLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Parameter")
    FString enemyRotation;

};


UCLASS()
class PEACEFULHAZARD_API UPeacFulSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    FVector SavedPlayerLocation;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    FRotator SavedPlayerRotation;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    FCharacterInventoty SavedPlayerInventory;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    FCharacterItemBox SavedPlayerCharacterItemBox;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    int32 SavedPlayerMaxBullet;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    int32 SavedPlayerCurrentBullet;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    float SavedPlayerCurrentHealth;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    EItemType SavedPlayercurrentBulletType;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    bool SavePlayerdEquipped;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    TMap<EWarpTarget, FString> MapInteractSaveMap;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    TMap<EWarpTarget, FEnemySave> EnemySaveMap;


};
