// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicEnemySpawner.generated.h"

class AEnemyBase;
class APeaceFulHazardGameMode;

UCLASS()
class PEACEFULHAZARD_API ADynamicEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawn Para")
	TSubclassOf<AEnemyBase> SpawnEnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawn Para")
	float SpawnTime = 3.f;


	APeaceFulHazardGameMode* PeaceFulHazardGameMode;


	UFUNCTION()
	void SpawnEnemy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
