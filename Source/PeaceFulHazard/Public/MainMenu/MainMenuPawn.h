// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PeaceFulHazard/PeaceFulHazard.h"

#include "MainMenuPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

class AMainMenuGameMode;

USTRUCT(BlueprintType)
struct FPawnCameraTransform
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	FVector PawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	FRotator CameraRotation;
};


UCLASS()
class PEACEFULHAZARD_API AMainMenuPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainMenuPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	AMainMenuGameMode* MainMenuGameMode;

	UFUNCTION()
	void ChangeCameraPosition(EMainMenuType menuType, bool bChangeStart);

	void LerpCameraTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera para")
	TMap<EMainMenuType, FPawnCameraTransform> PawnCameraTransforms;

	EMainMenuType currentMenuType = EMainMenuType::EMT_Default;
	EMainMenuType beforeMenuType = EMainMenuType::EMT_None;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
