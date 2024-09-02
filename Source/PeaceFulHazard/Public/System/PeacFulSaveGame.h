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
    float enemyHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Parameter")
    FVector enemyLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Parameter")
    FRotator enemyRotation;

};

USTRUCT(BlueprintType)
struct FSaveArrayContainer
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Parameter")
    TArray<FString> interactedItemNames;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Parameter")
    TMap<FString, FEnemySave> enemySaves;

    
};

UCLASS()
class PEACEFULHAZARD_API UPeacFulSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    int32 saveTodoIndex = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    FDateTime SaveTime;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    EWarpTarget saveMapName = EWarpTarget::EWT_None;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    TMap<ETutorialType, bool> TutorialCheckMap;

    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    bool bFirstGame = true;

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

    // ���⼭ �ڱ��� �̹� ��ȣ�ۿ� �߰ų� �׾����� �ƴ��� �ڱ� �̸����� Ȯ����
    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    TMap<EWarpTarget, FSaveArrayContainer> MapInteractSaveMap;

    // ���� ������ �ڱⰡ ���׾��ٰ� �ǴܵǸ� ���⿡ �����͸� ������
    UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
    TMap<EWarpTarget, FSaveArrayContainer> EnemySaveMap;


};
