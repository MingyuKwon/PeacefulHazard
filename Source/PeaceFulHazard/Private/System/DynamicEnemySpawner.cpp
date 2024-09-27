// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DynamicEnemySpawner.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADynamicEnemySpawner::ADynamicEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADynamicEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->DynamicSpawnStartEvent.AddDynamic(this, &ThisClass::SpawnEnemy);
	}

}

void ADynamicEnemySpawner::SpawnEnemy()
{
	if (SpawnEnemyClass)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {

			FVector Location = GetActorLocation();
			GetWorld()->SpawnActor<AEnemyBase>(SpawnEnemyClass, Location, GetActorRotation());
			Destroy();
		}, SpawnTime, false);
	}

}

// Called every frame
void ADynamicEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

