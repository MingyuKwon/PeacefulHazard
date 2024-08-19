// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PistolShell.generated.h"

UCLASS()
class PEACEFULHAZARD_API APistolShell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistolShell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
