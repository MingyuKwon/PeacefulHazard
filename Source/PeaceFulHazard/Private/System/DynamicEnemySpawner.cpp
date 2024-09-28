// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DynamicEnemySpawner.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ADynamicEnemySpawner::ADynamicEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	RootComponent = HeadBox;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh Board"));
	StaticMesh->SetupAttachment(RootComponent);
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

void ADynamicEnemySpawner::SetMaterialParaLerp(float value)
{
	if (StaticMesh)
	{
		StaticMesh->SetScalarParameterValueOnMaterials("Dissolve", value);
	}
}

void ADynamicEnemySpawner::SpawnEnemy()
{
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::TimerEnd, SpawnTime, false);
		
}

void ADynamicEnemySpawner::TimerEnd()
{
	if (SpawnEnemyClass)
	{
		GetWorld()->SpawnActor<AEnemyBase>(SpawnEnemyClass, GetActorLocation(), GetActorRotation());

	}

	bDestory = true;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
		Destroy();
		}, 0.5f, false);

	
}

// Called every frame
void ADynamicEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	acculumateValue += bDestory ? 0.02f : DeltaTime / (SpawnTime * 2);

	SetMaterialParaLerp(acculumateValue);
}

