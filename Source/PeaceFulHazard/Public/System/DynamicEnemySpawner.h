// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicEnemySpawner.generated.h"

class AEnemyBase;
class APeaceFulHazardGameMode;
class UBoxComponent;

UCLASS()
class PEACEFULHAZARD_API ADynamicEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicEnemySpawner();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HeadBox;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawn Para")
	TSubclassOf<AEnemyBase> SpawnEnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawn Para")
	float SpawnTime = 3.f;

	float acculumateValue = 0.f;

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;


	void SetMaterialParaLerp( float value);

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	void TimerEnd();

	bool bDestory = false;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
