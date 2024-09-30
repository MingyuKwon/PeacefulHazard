// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "MainMenu/MainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainMenuPawn::AMainMenuPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
	CameraBoom->TargetArmLength = 130.f;
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 

}

// Called when the game starts or when spawned
void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	MainMenuGameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));

	if (MainMenuGameMode)
	{
		MainMenuGameMode->MenuModeChangeEvent.AddDynamic(this, &ThisClass::ChangeCameraPosition);
	}
}

void AMainMenuPawn::ChangeCameraPosition(EMainMenuType menuType, bool bChangeStart)
{
	if (bChangeStart)
	{
		beforeMenuType = currentMenuType;
		currentMenuType = menuType;
	}
}

void AMainMenuPawn::LerpCameraTransform()
{
	if (!PawnCameraTransforms.Contains(currentMenuType)) return;
	if (beforeMenuType == EMainMenuType::EMT_None) return;

	FVector TargetLocation = PawnCameraTransforms[currentMenuType].PawnLocation;
	FRotator TargetRotation = PawnCameraTransforms[currentMenuType].CameraRotation;

	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentCameraRotation = FollowCamera->GetComponentRotation();

	FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, 0.05f);
	FRotator NewRotation = FMath::RInterpTo(CurrentCameraRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);

	SetActorLocation(NewLocation);
	FollowCamera->SetWorldRotation(NewRotation);

	float LocationErrorTolerance = 30.0f; 

	if (FVector::Dist(NewLocation, TargetLocation) < LocationErrorTolerance )
	{
		MainMenuGameMode->MenuModeChangeEvent.Broadcast(currentMenuType, false);
		beforeMenuType = EMainMenuType::EMT_None;
	}
}

// Called every frame
void AMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LerpCameraTransform();
}

