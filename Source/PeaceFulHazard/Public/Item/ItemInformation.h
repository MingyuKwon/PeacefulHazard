// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "ItemInformation.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FItemInformationUnit
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	EItemType itemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	int32 itemMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	FString itemNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	FString itemExplainText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EItemType, EItemType> ItemCombineMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	UTexture2D* itemImage;  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	bool bUsable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	bool bDiscardable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	bool bKeyItem;

};

USTRUCT(BlueprintType)
struct FSituationInformationUnit
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	EInteractSituationType situationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	FString situationNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	FString situationExplainText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TArray<EItemType> InteractItemArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	UTexture2D* situationImage;
};

UCLASS()
class PEACEFULHAZARD_API AItemInformation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemInformation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EItemType, FItemInformationUnit> ItemInformationMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	TMap<EInteractSituationType, FSituationInformationUnit> SituationMap;


};
