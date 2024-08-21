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

        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::RIghtClickStart);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::RightClickEnd);

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

    InitializeInventory();

    if (PlayerHUD)
    {
        PlayerHUD->SetBulletDisplay(currentBullet, maxBullet, GetLeftBullet());
        PlayerHUD->SetInventoryDisplay(&CharacterInventoty);
    }

}


void AHappyPlayerController::InitializeInventory()
{
    CharacterInventoty.ItemLockArray.Init(false, 10);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);

    CharacterInventoty.inventoryItems.Init(EItemType::EIT_None, 15);
    CharacterInventoty.inventoryItemCounts.Init(0, 15);
}

void AHappyPlayerController::Tab(const FInputActionValue& Value)
{
    ChangeInventoryState();
}

void AHappyPlayerController::ChangeInventoryState()
{
    if (PlayerHUD)
    {
        nowPausing = !nowPausing;

        PlayerHUD->SetTabDisplay(nowPausing);
        PauseGame(nowPausing);
    }
}

void AHappyPlayerController::PauseGame(bool flag)
{
    if (flag)
    {
        bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);

        SetPause(true);
    }
    else
    {
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());

        SetPause(false);
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

void AHappyPlayerController::RIghtClickStart(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (IsPaused())
    {
        if (PlayerHUD)
        {
            if (PlayerHUD->GetCanCloseTab())
            {
                ChangeInventoryState();
            }
            else
            {
                PlayerHUD->BackUIInputTrigger();
            }
            
        }

    }
    else
    {
        if (ControlledCharacter)
        {
            bSuccess = ControlledCharacter->AimStart(Value);
        }
    }


}

void AHappyPlayerController::RightClickEnd(const FInputActionValue& Value)
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


int32 AHappyPlayerController::GetReloadBulletCount()
{
    int32 neededBullet = maxBullet - currentBullet;

    neededBullet = FMath::Clamp(neededBullet, 0, GetLeftBullet());

    return neededBullet;
}

int32 AHappyPlayerController::GetLeftBullet()
{
    if (CharacterInventoty.ItemCountMap.Contains(EItemType::EIT_Bullet_Noraml))
    {
        return CharacterInventoty.ItemCountMap[EItemType::EIT_Bullet_Noraml];
    }

    return 0;
}

void AHappyPlayerController::UpdateDefaultUI()
{
    if (PlayerHUD)
    {
        PlayerHUD->SetBulletDisplay(currentBullet, maxBullet, GetLeftBullet());
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
        int32 reloadBulletCount = GetReloadBulletCount();
        currentBullet += reloadBulletCount;
        ChangeItemInventory(EItemType::EIT_Bullet_Noraml, -reloadBulletCount);

    }

    currentBullet = FMath::Clamp(currentBullet, 0, maxBullet);
    UpdateDefaultUI();
}

void AHappyPlayerController::ChangeItemInventory(EItemType itemType, int32 count)
{
    if (CharacterInventoty.ItemCountMap.Contains(itemType))
    {
        CharacterInventoty.ItemCountMap[itemType] += count;
    }
    else
    {
        if (count > 0)
        {
            CharacterInventoty.ItemCountMap.Add(itemType, count);
        }
        else
        {
            return;
        }

    }

    TArray<EItemType> ItemsToRemove;

    for (const TPair<EItemType, int32>& Pair : CharacterInventoty.ItemCountMap)
    {
        if (Pair.Value <= 0)
        {
            ItemsToRemove.Add(Pair.Key);
        }
    }

    for (EItemType Key : ItemsToRemove)
    {
        CharacterInventoty.ItemCountMap.Remove(Key);
    }
}
