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

        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::Action);

        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::AimStart);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::AimEnd);

        EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftStart);
        EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftEnd);

        EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ThisClass::EquipTrigger);

        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ThisClass::Reload);
        
        EnhancedInputComponent->BindAction(TabAction, ETriggerEvent::Started, this, &ThisClass::Tab);


        

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."));
    }
}

void AHappyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    PlayerHUD = Cast<APlayerHUD>(GetHUD());
    ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn());

    if (PlayerHUD)
    {
        PlayerHUD->SetBulletDisplay(currentBullet, maxBullet, GetLeftBulletFromCharacter());
    }

}

void AHappyPlayerController::Move(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter)
    {
        bSuccess = ControlledCharacter->Move(Value);
    }
}

void AHappyPlayerController::Look(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter)
    {
        bSuccess = ControlledCharacter->Look(Value);
    }
}

void AHappyPlayerController::Action(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter == nullptr) return;


    if (ControlledCharacter->GetIsAiming())
    {
        if (currentBullet <= 0) return;

        bSuccess = ControlledCharacter->Fire(Value);
       
        if (bSuccess)
        {
            SetBulletCount(true);
        }

    }
    else
    {
        ControlledCharacter->TriggerInteract();
    }

    UpdateDefaultUI();
}

void AHappyPlayerController::AimStart(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter)
    {
        bSuccess = ControlledCharacter->AimStart(Value);
    }
}

void AHappyPlayerController::AimEnd(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter)
    {
        bSuccess = ControlledCharacter->AimEnd(Value);
    }
}

void AHappyPlayerController::ShiftStart(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter)
    {
        bSuccess = ControlledCharacter->ShiftStart(Value);
    }
}

void AHappyPlayerController::ShiftEnd(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter)
    {
        bSuccess = ControlledCharacter->ShiftEnd(Value);
    }
}

void AHappyPlayerController::EquipTrigger(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter)
    {
        bSuccess = ControlledCharacter->EquipTrigger(Value);
    }
}

void AHappyPlayerController::Reload(const FInputActionValue& Value)
{
    bool bSuccess = false;
    int32 reloadBulletCount = GetReloadBulletCount();

    if (reloadBulletCount <= 0) return;
    
    if (ControlledCharacter)
    {
        bSuccess = ControlledCharacter->Reload(Value);
    }

}

void AHappyPlayerController::Tab(const FInputActionValue& Value)
{
    bool bSuccess = false;
    if (PlayerHUD)
    {
        nowPausing = !nowPausing;

        PlayerHUD->SetTabDisplay(nowPausing);

    }

    if (nowPausing)
    {
        bShowMouseCursor = true;
        SetInputMode(FInputModeUIOnly());

        SetPause(true);
    }
    else
    {
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());

        SetPause(false);
    }
}



int32 AHappyPlayerController::GetReloadBulletCount()
{
    int32 neededBullet = maxBullet - currentBullet;

    neededBullet = FMath::Clamp(neededBullet, 0, GetLeftBulletFromCharacter());

    return neededBullet;
}

int32 AHappyPlayerController::GetLeftBulletFromCharacter()
{
    if (ControlledCharacter)
    {
        return ControlledCharacter->GetLeftBullet();
    }

    return 0;
}

void AHappyPlayerController::UpdateDefaultUI()
{
    if (PlayerHUD)
    {
        PlayerHUD->SetBulletDisplay(currentBullet, maxBullet, GetLeftBulletFromCharacter());
    }
}

void AHappyPlayerController::SetBulletCount(bool bFire)
{
    if (bFire)
    {
        currentBullet--;
    }
    else
    {
        if (ControlledCharacter)
        {
            int32 reloadBulletCount = GetReloadBulletCount();

            currentBullet += reloadBulletCount;

            ControlledCharacter->ChangeItemInventory(EItemType::EIT_Bullet_Noraml, -reloadBulletCount);
        }
    }

    currentBullet = FMath::Clamp(currentBullet, 0, maxBullet);
    UpdateDefaultUI();
}
