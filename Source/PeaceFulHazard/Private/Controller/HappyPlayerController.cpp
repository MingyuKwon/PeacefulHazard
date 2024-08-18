// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/HappyPlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UI/PlayerHUD.h"
#include "Controller/HappyPlayerController.h"
#include "Battle/Weapon.h"
#include "Character/PeaceFulHazardCharacter.h"

void AHappyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::Fire);

        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::AimStart);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::AimEnd);

        EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftStart);
        EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftEnd);

        EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ThisClass::EquipTrigger);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."));
    }
}

void AHappyPlayerController::Move(const FInputActionValue& Value)
{
    if (APeaceFulHazardCharacter* ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn()))
    {
        ControlledCharacter->Move(Value);
    }
}

void AHappyPlayerController::Look(const FInputActionValue& Value)
{
    if (APeaceFulHazardCharacter* ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn()))
    {
        ControlledCharacter->Look(Value);
    }
}

void AHappyPlayerController::Fire(const FInputActionValue& Value)
{
    if (APeaceFulHazardCharacter* ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn()))
    {
        ControlledCharacter->Fire(Value);
    }
}

void AHappyPlayerController::AimStart(const FInputActionValue& Value)
{
    if (APeaceFulHazardCharacter* ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn()))
    {
        ControlledCharacter->AimStart(Value);
    }
}

void AHappyPlayerController::AimEnd(const FInputActionValue& Value)
{
    if (APeaceFulHazardCharacter* ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn()))
    {
        ControlledCharacter->AimEnd(Value);
    }
}

void AHappyPlayerController::ShiftStart(const FInputActionValue& Value)
{
    if (APeaceFulHazardCharacter* ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn()))
    {
        ControlledCharacter->ShiftStart(Value);
    }
}

void AHappyPlayerController::ShiftEnd(const FInputActionValue& Value)
{
    if (APeaceFulHazardCharacter* ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn()))
    {
        ControlledCharacter->ShiftEnd(Value);
    }
}

void AHappyPlayerController::EquipTrigger(const FInputActionValue& Value)
{
    if (APeaceFulHazardCharacter* ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn()))
    {
        ControlledCharacter->EquipTrigger(Value);
    }
}
