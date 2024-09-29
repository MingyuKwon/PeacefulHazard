// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "MainMenu/MainMenuHUD.h"

#include "MainMenu/MainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

        EnhancedInputComponent->BindAction(RIghtClickAction, ETriggerEvent::Triggered, this, &ThisClass::RIghtClick);
        //EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

        

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."));
    }
}

void AMainMenuController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;

    MainMenuGameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(this));
    MainMenuHUD = Cast<AMainMenuHUD>(GetHUD());

}

void AMainMenuController::ChangeCameraPosition(EMainMenuType menuType, bool bChangeStart)
{
    if (menuType == EMainMenuType::EMT_Load)
    {
        if (MainMenuGameMode)
        {
            MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Load, true);
        }
    }
    else if (menuType == EMainMenuType::EMT_Setting)
    {
        if (MainMenuGameMode)
        {
            MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Setting, true);
        }
    }
    else if (menuType == EMainMenuType::EMT_NewGame)
    {
        if (MainMenuGameMode)
        {
            MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_NewGame, true);

        }
    }
    else if (menuType == EMainMenuType::EMT_Default)
    {
        if (MainMenuGameMode)
        {
            MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Default, true);

        }
    }
    else if (menuType == EMainMenuType::EMT_GameStart)
    {
        if (MainMenuGameMode)
        {
            MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_GameStart, true);

        }
    }
}

void AMainMenuController::RIghtClick(const FInputActionValue& Value)
{
    if (MainMenuHUD)
    {
        MainMenuHUD->BackUIInputTrigger();
    }
}

void AMainMenuController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementVector = MovementVector.GetSafeNormal();

    if (MovementVector.X <= -0.5f)
    {
        if (MainMenuGameMode)
        {
            MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Load, true);
        }
    }
    else if (MovementVector.X >= 0.5f)
    {
        if (MainMenuGameMode)
        {
            MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Setting, true);
        }
    }
    else if (MovementVector.Y <= -0.5f)
    {
        if (MainMenuGameMode)
        {
            MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_NewGame, true);

        }
    }
    else if (MovementVector.Y >= 0.5f)
    {
        if (MainMenuGameMode)
        {
            MainMenuGameMode->MenuModeChangeEvent.Broadcast(EMainMenuType::EMT_Default, true);

        }
    }
}
