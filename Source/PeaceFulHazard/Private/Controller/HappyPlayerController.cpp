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
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Item/HappyInteractableItem.h"
#include "System/PeacFulGameInstance.h"

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
        EnhancedInputComponent->BindAction(BulletChangeAction, ETriggerEvent::Started, this, &ThisClass::ChangeBullet);

        EnhancedInputComponent->BindAction(TabAction, ETriggerEvent::Started, this, &ThisClass::Tab);
        EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &ThisClass::Menu);

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."));
    }
}

void AHappyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    ItemInformation = GetWorld()->SpawnActor<AItemInformation>(ItemInformationClass);

    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());


    PlayerHUD = Cast<APlayerHUD>(GetHUD());
    ControlledCharacter = Cast<APeaceFulHazardCharacter>(GetPawn());
    PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    PeacFulGameInstance = Cast<UPeacFulGameInstance>(UGameplayStatics::GetGameInstance(this));

    if (PeaceFulHazardGameMode)
    {
        PeaceFulHazardGameMode->OuterChangeInventoryEvent.AddDynamic(this, &ThisClass::OuterUIChange);
        PeaceFulHazardGameMode->UseItemEvent.AddDynamic(this, &ThisClass::UseItem);
        PeaceFulHazardGameMode->CloseAllUIEvent.AddDynamic(this, &ThisClass::CloseAllUI);
        PeaceFulHazardGameMode->InteractWithItemUIEvent.AddDynamic(this, &ThisClass::InteractWithItemUI);
        PeaceFulHazardGameMode->InteractSituationEvent.AddDynamic(this, &ThisClass::SituationInteract);
        PeaceFulHazardGameMode->ItemBoxInteractEvent.AddDynamic(this, &ThisClass::ChangeItemBox);

        PeaceFulHazardGameMode->NoticeUIShowEvent.AddDynamic(this, &ThisClass::ShowNoticeUI);
        PeaceFulHazardGameMode->InformationUIShowEvent.AddDynamic(this, &ThisClass::ShowInformationUI);

        PeaceFulHazardGameMode->TutorialEvent.AddDynamic(this, &ThisClass::TutorialShow);
        PeaceFulHazardGameMode->MapTravelEvent.AddDynamic(this, &ThisClass::WarpTravel);
        PeaceFulHazardGameMode->MapStartEvent.AddDynamic(this, &ThisClass::MapStartCallBack);

        PeaceFulHazardGameMode->MenuUIChangeEvent.AddDynamic(this, &ThisClass::ShowMainMenuUI);

        PeaceFulHazardGameMode->WantSaveEvent.AddDynamic(this, &ThisClass::WantToSaveCallBack);

        PeaceFulHazardGameMode->CinematicPlayEvent.AddDynamic(this, &ThisClass::CinematicShow);


        PeaceFulHazardGameMode->ShowLoadingEvent.AddDynamic(this, &ThisClass::ShowLoadingUI);


        PeaceFulHazardGameMode->UpdateDefaultUIEvent.AddDynamic(this, &ThisClass::UpdateAllUI);

        PeaceFulHazardGameMode->CheckOneMoreGameEvent.AddDynamic(this, &ThisClass::SetOnceCheckDisplay);

        
        InitializeInventory();
    }

    if (PlayerHUD)
    {
        UpdateAllUI();

        PlayerHUD->ShowLoadingUI(true);

        bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);

        FTimerHandle loadingHandle;
        GetWorld()->GetTimerManager().SetTimer(loadingHandle, [this]()
            {
                bShowMouseCursor = false;
                SetInputMode(FInputModeGameOnly());
                PlayerHUD->ShowLoadingUI(false);
            }, 1.5f, false);

    }

    GetWorld()->GetTimerManager().SetTimer(UpdateValueHandle, this, &AHappyPlayerController::UpdateValue, 0.02f, true);

    FTimerHandle DefaultUpdateHandle;
    GetWorld()->GetTimerManager().SetTimer(DefaultUpdateHandle, this, &AHappyPlayerController::UpdateDefaultUI, 2.f, true);

    
    
}

void AHappyPlayerController::UpdateValue()
{
    if (ControlledCharacter == nullptr) return;

    if (ControlledCharacter->GetIsAiming())
    {
        aimFocusLerpValue = FMath::Lerp(aimFocusLerpValue, 0, aimFocusLerpValue < 0.5f ? 0.03f : 0.05f);

        if (ControlledCharacter->GetPlayerSpeed() < 0.1f)
        {
        }
        else
        {
            aimFocusLerpValue = FMath::Clamp(aimFocusLerpValue, 0.3, 1.f);
        }

    }
    else
    {
        aimFocusLerpValue = FMath::Lerp(aimFocusLerpValue, 1, 1.f);
    }



    if (PlayerHUD)
    {
        PlayerHUD->SetAimWide(aimFocusLerpValue);
    }
}

void AHappyPlayerController::SetOnceCheckDisplay(bool bVisible, const FText EnglishText, const FText KoreanText)
{
    if (PlayerHUD)
    {
        PlayerHUD->SetOnceCheckDisplay(bVisible,  EnglishText, KoreanText);
    }
}

void AHappyPlayerController::InitializeInventory()
{
    CharacterInventoty.ItemLockArray.Init(false, 5);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);
    CharacterInventoty.ItemLockArray.Add(true);

    CharacterInventoty.inventoryItems.Init(EItemType::EIT_None, 15);
    CharacterInventoty.inventoryItemCounts.Init(0, 15);


    CharacterItemBox.itemBoxItems.Init(EItemType::EIT_None, 20);
    CharacterItemBox.itemBoxItemCounts.Init(0, 20);

}


void AHappyPlayerController::Tab(const FInputActionValue& Value)
{
    if (bCinematicShow) return;

    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

    }

    if (PlayerHUD)
    {
        if (currentUIState == EUIState::EUIS_Notice || currentUIState == EUIState::EUIS_Menu) return;

        ChangeUiState(EUIState::EUIS_Tab, currentUIState != EUIState::EUIS_Tab);

        if (currentUIState == EUIState::EUIS_Tab)
        {


            UpdateAllUI();

            PlayerHUD->SetInventoryDisplay(true);

            TutorialShow(ETutorialType::ETT_UIShowInventory);


        }
        else
        {
            PlayerHUD->SetInventoryDisplay(false);
        }

    }
}

void AHappyPlayerController::Menu(const FInputActionValue& Value)
{
    if (bCinematicShow) return;
    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

    }

    if (PlayerHUD)
    {
        if (currentUIState == EUIState::EUIS_Notice) return;

        UpdateAllUI();

        ChangeUiState(EUIState::EUIS_Menu, currentUIState != EUIState::EUIS_Menu);


        if (currentUIState == EUIState::EUIS_Menu)
        {
            PlayerHUD->SetMainMenuDisplay(true, EMenuType::EMT_Map);
        }
        else
        {
            PlayerHUD->SetMainMenuDisplay(false, EMenuType::EMT_Map);
        }

    }
}


void AHappyPlayerController::GetItem(EItemType itemType, int32 count)
{
    if (PlayerHUD)
    {
        ChangeUiState(EUIState::EUIS_Tab, true);


        UpdateAllUI();


        PlayerHUD->SetGetItemDisplay(true, itemType, count);

        TutorialShow(ETutorialType::ETT_UIGetItem);

    }
}

void AHappyPlayerController::TriggerSituation(EInteractSituationType situtaionType)
{
    if (PlayerHUD)
    {
        ChangeUiState(EUIState::EUIS_Tab, true);

        UpdateAllUI();


        PlayerHUD->showSituationUI(true, situtaionType);

        TutorialShow(ETutorialType::ETT_UISituation);

    }
}

void AHappyPlayerController::TriggerItemBox()
{
    if (PlayerHUD)
    {
        if (currentUIState == EUIState::EUIS_Notice) return;

        ChangeUiState(EUIState::EUIS_Tab, true);


        UpdateAllUI();


        if (currentUIState == EUIState::EUIS_Tab)
        {
            PlayerHUD->SetItemBoxDisplay(true);
        }
        else
        {
            PlayerHUD->SetInventoryDisplay(false);
        }


        TutorialShow(ETutorialType::ETT_UIItemBox);

    }
}

void AHappyPlayerController::TriggerMenu_Save(bool bSave)
{
    if (PlayerHUD)
    {
        if (currentUIState == EUIState::EUIS_Notice) return;

        ChangeUiState(EUIState::EUIS_Menu, true);

        UpdateAllUI();

        if (currentUIState == EUIState::EUIS_Menu)
        {
            PlayerHUD->SetMainMenuDisplay(true, EMenuType::EMT_Save, bSave);
        }
        else
        {
            PlayerHUD->SetMainMenuDisplay(false, EMenuType::EMT_Save, bSave);
        }

    }
}

void AHappyPlayerController::SetHealth(float changeAmount)
{
    currentHealth += changeAmount;
    currentHealth = FMath::Clamp(currentHealth, 0, 50);

    UpdateDefaultUI();

    if (ControlledCharacter)
    {
        if (currentHealth <= 0)
        {
            ControlledCharacter->Death();

            bShowMouseCursor = true;

            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            SetInputMode(InputMode);


            FTimerHandle timerHandle;
            GetWorld()->GetTimerManager().SetTimer(timerHandle, [this]() {
                if (PlayerHUD)
                {
                    PlayerHUD->SetGameOverDisplay(true);
                }
                SetGamePause(true);
                }, 2.5f, false);

        }
    }
}

void AHappyPlayerController::GameClear()
{
    bShowMouseCursor = true;


    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(InputMode);

    if (PlayerHUD)
    {
       PlayerHUD->SetGameOverDisplay(true, true);
    }
 }

void AHappyPlayerController::TakeDamge(float damage)
{
    if (PeacFulGameInstance == nullptr) return;

    int32 gap = GetPlayerForce() - PeacFulGameInstance->currentEnemyForce;
    float percent = 1 - gap / 100.f;

    damage *= percent;

    SetHealth(-damage);

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
    if (bCinematicShow) return;

    bool bSuccess = false;

    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

        bSuccess = ControlledCharacter->Move(Value);
    }
}

void AHappyPlayerController::Look(const FInputActionValue& Value)
{
    if (bCinematicShow) return;

    bool bSuccess = false;

    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

        bSuccess = ControlledCharacter->Look(Value);
    }
}

void AHappyPlayerController::Action(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter == nullptr) return;

    if (IsPaused())
    {
        if (PlayerHUD)
        {
            if (PlayerHUD->GetOneMoreCheckVisible())
            {
                PlayerHUD->OneMoreCheckOkUIInputTrigger();
                return;
            }


            if (currentUIState == EUIState::EUIS_Notice)
            {
                PlayerHUD->BackNoticeUIInputTrigger();
            }
            else
            {
                PlayerHUD->OkUIInputTrigger();
            }
        }

        return;
    }

    if (bCinematicShow) return;
    if (ControlledCharacter->bDeath) return;


    if (ControlledCharacter->GetIsAiming())
    {
        if (currentBullet <= 0) return;

        bSuccess = ControlledCharacter->Fire(Value);
       
        if (bSuccess)
        {
            if (currentBulletType == EItemType::EIT_Bullet_Noraml)
            {
                aimFocusLerpValue = FMath::Clamp(aimFocusLerpValue, 0.6, 1.f);
            }
            else
            {
                aimFocusLerpValue = FMath::Clamp(aimFocusLerpValue, 1.0, 1.f);
            }

            SetBulletCount(true);
        }

    }
    else
    {
        ControlledCharacter->TriggerInteract();
    }

    UpdateAllUI();
}

void AHappyPlayerController::RIghtClickStart(const FInputActionValue& Value)
{

    bool bSuccess = false;

    if (IsPaused())
    {
        if (PlayerHUD)
        {
            if (PlayerHUD->GetOneMoreCheckVisible())
            {
                PlayerHUD->OneMoreCheckBackUIInputTrigger();
                return;
            }

            if (currentUIState == EUIState::EUIS_Notice)
            {
                PlayerHUD->BackNoticeUIInputTrigger();
            }
            else if (currentUIState == EUIState::EUIS_Menu)
            {
                PlayerHUD->BackMenuUIInputTrigger();
            }
            else
            {
                PlayerHUD->BackUIInputTrigger();
            }

        }

    }
    else
    {
        if (bCinematicShow) return;

        if (ControlledCharacter)
        {
            if (ControlledCharacter->bDeath) return;

            bSuccess = ControlledCharacter->AimStart(Value);
        }
    }


}

void AHappyPlayerController::RightClickEnd(const FInputActionValue& Value)
{
    if (bCinematicShow) return;

    bool bSuccess = false;

    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

        bSuccess = ControlledCharacter->AimEnd(Value);
    }
}

void AHappyPlayerController::ShiftStart(const FInputActionValue& Value)
{
    if (bCinematicShow) return;

    bool bSuccess = false;

    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

        bSuccess = ControlledCharacter->ShiftStart(Value);
    }
}

void AHappyPlayerController::ShiftEnd(const FInputActionValue& Value)
{
    if (bCinematicShow) return;

    bool bSuccess = false;

    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

        bSuccess = ControlledCharacter->ShiftEnd(Value);
    }
}

void AHappyPlayerController::EquipTrigger(const FInputActionValue& Value)
{
    if (bCinematicShow) return;

    bool bSuccess = false;

    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

        bSuccess = ControlledCharacter->EquipTrigger(currentBulletType);
    }
}

void AHappyPlayerController::Reload(const FInputActionValue& Value)
{
    if (bCinematicShow) return;

    bool bSuccess = false;
    int32 reloadBulletCount = GetReloadBulletCount();

    if (reloadBulletCount <= 0) return;
    
    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

        bSuccess = ControlledCharacter->Reload(Value);
    }

}

void AHappyPlayerController::ChangeBullet(const FInputActionValue& Value)
{
    if (bCinematicShow) return;

    bool bSuccess = false;

    if (currentBulletType == EItemType::EIT_Bullet_Noraml && !CharacterInventoty.ItemCountMap.Contains(EItemType::EIT_Bullet_Big)) return;
    if (currentBulletType == EItemType::EIT_Bullet_Big && !CharacterInventoty.ItemCountMap.Contains(EItemType::EIT_Bullet_Noraml)) return;

    

    if (ControlledCharacter)
    {
        if (ControlledCharacter->bDeath) return;

        bSuccess = ControlledCharacter->ChangeBullet();
    }
   
}


int32 AHappyPlayerController::GetReloadBulletCount()
{
    int32 neededBullet = maxBullet - currentBullet;

    neededBullet = FMath::Clamp(neededBullet, 0, GetLeftBullet());

    return neededBullet;
}



void AHappyPlayerController::SetGamePause(bool flag)
{
    nowPausing = flag;
    PauseGame(nowPausing);
}

int32 AHappyPlayerController::GetPlayerForce()
{
    if (CharacterInventoty.ItemCountMap.Contains(EItemType::EIT_Force))
    {
        return CharacterInventoty.ItemCountMap[EItemType::EIT_Force];
    }

    return 0;
}

int32 AHappyPlayerController::GetLeftBullet()
{
    if (CharacterInventoty.ItemCountMap.Contains(currentBulletType))
    {
        return CharacterInventoty.ItemCountMap[currentBulletType];
    }

    return 0;
}

int32 AHappyPlayerController::GetAnotherBullet()
{
    EItemType anotherItemType ;

    if (currentBulletType == EItemType::EIT_Bullet_Big)
    {
        anotherItemType = EItemType::EIT_Bullet_Noraml;
    }
    else
    {
        anotherItemType = EItemType::EIT_Bullet_Big;
    }

    if (CharacterInventoty.ItemCountMap.Contains(anotherItemType))
    {
        return CharacterInventoty.ItemCountMap[anotherItemType];
    }

    return 0;
}

int32 AHappyPlayerController::GetLockIndex()
{
    int32 Lockindex = 0;
    for (bool Lock : CharacterInventoty.ItemLockArray)
    {
        if (Lock)
        {
            break;
        }

        Lockindex++;
    }

    return Lockindex;
}

void AHappyPlayerController::UpdateAllUI()
{
    UpdateInventoryUI();
    UpdateDefaultUI();
    UpdateItemBoxUI();
}

void AHappyPlayerController::UpdateInventoryUI()
{
    if (PlayerHUD)
    {
        PlayerHUD->UpdateInventoryDisplay(&CharacterInventoty);
    }
}

void AHappyPlayerController::UpdateItemBoxUI()
{
    if (PlayerHUD)
    {
        PlayerHUD->UpdateItemBoxDisplay(&CharacterInventoty, &CharacterItemBox);
    }
}

void AHappyPlayerController::UpdateDefaultUI()
{
    if (PlayerHUD)
    {
        PlayerHUD->UpdateBulletDisplay(currentBullet, maxBullet, GetLeftBullet(), currentBulletType, GetAnotherBullet());
        PlayerHUD->UpdateTodoUI();
        PlayerHUD->UpdateHealthUI(currentHealth / 50.f, GetPlayerForce());
        
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
        ChangeItemInventory(currentBulletType, -reloadBulletCount);

    }

    currentBullet = FMath::Clamp(currentBullet, 0, maxBullet);
    UpdateAllUI();
}

void AHappyPlayerController::SetBulletChangeCount()
{
    int32 beforeCurrentBullet = currentBullet;

    if (currentBulletType == EItemType::EIT_Bullet_Noraml)
    {
        currentBulletType = EItemType::EIT_Bullet_Big;
        maxBullet = BigBulletMax;
    }
    else
    {
        currentBulletType = EItemType::EIT_Bullet_Noraml;
        maxBullet = normalBulletMax;
    }

    int32 nowLeftBullet = GetLeftBullet();
    int32 reloadBullet = FMath::Clamp(nowLeftBullet, 0, maxBullet);

    currentBullet = reloadBullet;
    ChangeItemInventory(currentBulletType, -reloadBullet);

    if (currentBulletType == EItemType::EIT_Bullet_Noraml)
    {
        ChangeItemInventory(EItemType::EIT_Bullet_Big, beforeCurrentBullet);
    }
    else
    {
        ChangeItemInventory(EItemType::EIT_Bullet_Noraml, beforeCurrentBullet);

    }

    UpdateAllUI();
}



void AHappyPlayerController::WantToSaveCallBack()
{
    if (ControlledCharacter && PeaceFulHazardGameMode)
    {
        PeaceFulHazardGameMode->SavePlayerPara(CharacterInventoty, CharacterItemBox, maxBullet, currentBullet, currentHealth, currentBulletType, ControlledCharacter->GetIEquipped(), ControlledCharacter->GetActorLocation(), ControlledCharacter->GetActorRotation());

    }

    PeaceFulHazardGameMode->SetEnemySaveRefCount(false);
}

void AHappyPlayerController::ChangeUiState(EUIState uiState, bool bLock)
{
    FString EnumAsString = UEnum::GetValueAsString(uiState);
    UE_LOG(LogTemp, Warning, TEXT("Input UI State: %s, %s"), *EnumAsString, bLock ? *FString("bLock = true") : *FString("bLock = false"));

    if (uiState == EUIState::EUIS_Notice)
    {
        bNoticeLock = bLock;
    }
    else if (uiState == EUIState::EUIS_Menu)
    {
        bMenuLock = bLock;
    }
    else 
    {
        bTabLock = bLock;
    }


    if (bNoticeLock)
    {
        currentUIState = EUIState::EUIS_Notice;
        
    }
    else if (bMenuLock)
    {
        currentUIState = EUIState::EUIS_Menu;
    }
    else if (bTabLock)
    {

        currentUIState = EUIState::EUIS_Tab;
    }
    else
    {
        currentUIState = EUIState::EUIS_None;
    }

    EnumAsString = UEnum::GetValueAsString(currentUIState);
    UE_LOG(LogTemp, Warning, TEXT("Current UI State: %s"), *EnumAsString);

    if (currentUIState == EUIState::EUIS_None)
    {
        PauseGame(false);
    }
    else
    {
        if (!nowPausing)
        {
            PauseGame(true);
        }
    }
}

void AHappyPlayerController::CloseAllUI()
{
    ChangeUiState(EUIState::EUIS_None, false);

    if (currentUIState == EUIState::EUIS_None)
    {
        PlayerHUD->SetGetItemDisplay(false);
        PlayerHUD->SetInventoryDisplay(false);
        PlayerHUD->showSituationUI(false);
        PlayerHUD->SetItemBoxDisplay(false);

    }
}

void AHappyPlayerController::MapStartCallBack()
{
    bool isSavedDataRemain = false;
    bool bEquipped = false;

    FVector Positoin;
    FRotator Rotation;
    EWarpTarget warptarget;

    isSavedDataRemain = PeaceFulHazardGameMode->GetPlayerPara(CharacterInventoty, CharacterItemBox, maxBullet, currentBullet, currentHealth, currentBulletType, bEquipped, Positoin, Rotation, warptarget);

    if (isSavedDataRemain)
    {
       
        if (warptarget == PeaceFulHazardGameMode->currentMapType)
        {
            if (ControlledCharacter)
            {
                ControlledCharacter->SetActorLocation(Positoin);
                ControlledCharacter->SetActorRotation(Rotation);
            }
        }

        if (bEquipped && ControlledCharacter)
        {
            ControlledCharacter->EquipTrigger(currentBulletType);
        }

        if (ControlledCharacter)
        {
            ControlledCharacter->MapStartInitialize();
        }

        
    }

    UpdateAllUI();

}

void AHappyPlayerController::OuterUIChange(int32 itemIndex, EItemType itemType, int32 itemCount, bool bReplace)
{
    if (itemCount == 0) return;
    if (!ChangeItemInventoryArrayOneSlot(itemIndex, itemType, itemCount, bReplace)) return;
    if (!ChangeItemInventoryMap(itemType, itemCount)) return;

    UpdateAllUI();
}

void AHappyPlayerController::UseItem(EItemType itemType, bool bItem)
{
    switch (itemType)
    {
    case EItemType::EIT_HealPack_Small :
        SetHealth(20);
        break;

    case EItemType::EIT_HealPack_Big:
        SetHealth(50);
        break;
    }
}

void AHappyPlayerController::InteractWithItemUI(EItemType itemtype, int32 count)
{

    if (itemtype == EItemType::EIT_Bag)
    {
        InventorySizeUpgrade(count);
    }
    else
    {
        ChangeItemInventory(itemtype, count);
    }
    UpdateAllUI();

    CloseAllUI();
}

void AHappyPlayerController::SituationInteract(EInteractSituationType situationType)
{
    CloseAllUI();
}

void AHappyPlayerController::ShowLoadingUI(bool bVisible)
{
    if (PlayerHUD)
    {
        PlayerHUD->ShowLoadingUI(bVisible);
    }
}



bool AHappyPlayerController::ChangeItemInventoryArrayOneSlot(int32 itemIndex, EItemType itemType, int32 itemCount, bool bReplace)
{
    if (bReplace)
    {
        CharacterInventoty.inventoryItems[itemIndex] = itemType;
        CharacterInventoty.inventoryItemCounts[itemIndex] = itemCount;

    }
    else
    {
        if (CharacterInventoty.inventoryItems[itemIndex] != itemType) return false;

        CharacterInventoty.inventoryItemCounts[itemIndex] += itemCount;

        if (CharacterInventoty.inventoryItemCounts[itemIndex] <= 0)
        {
            CharacterInventoty.inventoryItems[itemIndex] = EItemType::EIT_None;
        }

        if (CharacterInventoty.inventoryItemCounts[itemIndex] > ItemInformation->ItemInformationMap[itemType].itemMaxCount)
        {
            if (!ChangeItemInventoryArray(itemType, CharacterInventoty.inventoryItemCounts[itemIndex] - ItemInformation->ItemInformationMap[itemType].itemMaxCount)) return false;

            CharacterInventoty.inventoryItemCounts[itemIndex] = ItemInformation->ItemInformationMap[itemType].itemMaxCount;
        }
    }

    UpdateAllUI();

    return true;
}

void AHappyPlayerController::ChangeItemInventory(EItemType itemType, int32 count)
{
    if (count == 0) return;
    if (!ChangeItemInventoryArray(itemType, count)) return;
    if (!ChangeItemInventoryMap(itemType, count)) return;
    
    UpdateAllUI();
}

void AHappyPlayerController::InventorySizeUpgrade(int32 count)
{
    int32 Lockindex = 0;
    for (bool Lock : CharacterInventoty.ItemLockArray)
    {
        if (Lock)
        {
            break;
        }

        Lockindex++;
    }

    Lockindex = FMath::Clamp(Lockindex, 0, CharacterInventoty.ItemLockArray.Num()-1);

    while (count > 0)
    {
        CharacterInventoty.ItemLockArray[Lockindex] = false;
        Lockindex++;
        Lockindex = FMath::Clamp(Lockindex, 0, CharacterInventoty.ItemLockArray.Num() - 1);
        count--;
    }

    UpdateAllUI();
}

bool AHappyPlayerController::CanGetItem(EItemType itemType, int32 count)
{
    if (!IsInventoryFull()) return true;
    if (itemType == EItemType::EIT_Bag) return true;

    // only if inventory is Full

    int32 MaxCountUnit = ItemInformation->ItemInformationMap[itemType].itemMaxCount;
    int32 FreeSpace = 0;

    for (int i = 0; i < 15; i++)
    {
        if (CharacterInventoty.inventoryItems[i] != itemType) continue;

        FreeSpace += MaxCountUnit - CharacterInventoty.inventoryItemCounts[i];
    }

    return FreeSpace >= count;
}

bool AHappyPlayerController::IsInventoryFull()
{

    int32 Lockindex = 0;
    for (bool Lock : CharacterInventoty.ItemLockArray)
    {
        if (Lock)
        {
            break;
        }

        Lockindex++;
    }

    int32 Emptyindex = 0;
    for (EItemType InventoryitemType : CharacterInventoty.inventoryItems)
    {
        if (InventoryitemType == EItemType::EIT_None)
        {
            break;
        }

        Emptyindex++;
    }

    return Emptyindex >= Lockindex;

}

void AHappyPlayerController::CinematicShow(bool bShow)
{
    bCinematicShow = bShow;

    if (bShow)
    {
        if (PlayerHUD)
        {
            PlayerHUD->SetDefaultDisplay(false);
        }

        if (ControlledCharacter && PeaceFulHazardGameMode && PeaceFulHazardGameMode->GetPlayerToDo() == EPlayerToDo::EPTD_GetOutTutorialRoom)
        {


            ControlledCharacter->bDissolveControllerControl = true;
        }
    }
    else
    {
        if (PlayerHUD)
        {
            PlayerHUD->SetDefaultDisplay(true);
        }

        if (PeaceFulHazardGameMode)
        {
            if (PeaceFulHazardGameMode->GetPlayerToDo() == EPlayerToDo::EPTD_Survive)
            {
                if (PeaceFulHazardGameMode->bGameClear)
                {
                    GameClear();
                }
                else
                {
                    TutorialShow(ETutorialType::ETT_FinalBattleTimeLimit);
                }
            }
            else
            {
                TutorialShow(ETutorialType::ETT_MoveTutorial);
            }

            if (ControlledCharacter && PeaceFulHazardGameMode->GetPlayerToDo() == EPlayerToDo::EPTD_GetOutTutorialRoom)
            {
                ControlledCharacter->bDissolveControllerControl = false;
            }
        }

        
    }

    
}

void AHappyPlayerController::ChangeItemBoxInContrller(int32 index, EItemType itemType, int32 count)
{
    if (count >= 0)
    {
        int32 Emptyindex = 0;
        for (EItemType boxItemType : CharacterItemBox.itemBoxItems)
        {
            if (boxItemType == EItemType::EIT_None) break;
            Emptyindex++;
        }
        
        if (Emptyindex >= 20) return;

        CharacterItemBox.itemBoxItems[Emptyindex] = itemType;
        CharacterItemBox.itemBoxItemCounts[Emptyindex] = count;

    }
    else
    {
        CharacterItemBox.itemBoxItems[index] = EItemType::EIT_None;
        CharacterItemBox.itemBoxItemCounts[index] = 0;

        ReArrangeBox();

    }
}

void AHappyPlayerController::ReArrangeBox()
{
    int samllestNoneIndex = 0;

    for (int i = 0; i < 20; i++)
    {
        if (CharacterItemBox.itemBoxItems[i] != EItemType::EIT_None)
        {
            if (samllestNoneIndex < i)
            {
                CharacterItemBox.itemBoxItems[samllestNoneIndex] = CharacterItemBox.itemBoxItems[i];
                CharacterItemBox.itemBoxItemCounts[samllestNoneIndex] = CharacterItemBox.itemBoxItemCounts[i];

                CharacterItemBox.itemBoxItems[i] = EItemType::EIT_None;
                CharacterItemBox.itemBoxItemCounts[i] = 0;

            }

            samllestNoneIndex++;

        }

    }
}

void AHappyPlayerController::ChangeItemBox(bool bInventroyToBox, int32 index, EItemType itemtype, int32 count)
{
    if (bInventroyToBox)
    {
        OuterUIChange(index, itemtype, -count, false);
        ChangeItemBoxInContrller(index, itemtype, count);
    }
    else
    {
        ChangeItemInventory(itemtype, count);
        ChangeItemBoxInContrller(index, itemtype, -count);
    }

    UpdateAllUI();
}

void AHappyPlayerController::ShowNoticeUI(bool bVisible, FString& noticeText)
{
    ChangeUiState(EUIState::EUIS_Notice, bVisible);

    if (bVisible)
    {
        if (PlayerHUD)
        {
            PlayerHUD->UpdateNoticeDisplay(noticeText);
            PlayerHUD->SetNoticeDisplay(true);
        }
    }
    else
    {
        if (PlayerHUD)
        {
            PlayerHUD->SetNoticeDisplay(false);

        }
    }
}

void AHappyPlayerController::ShowInformationUI(bool bVisible, FString& noticeText)
{
    ChangeUiState(EUIState::EUIS_Notice, bVisible);

    if (bVisible)
    {
        if (PlayerHUD)
        {
            PlayerHUD->UpdateInformationDisplay(noticeText);
            PlayerHUD->SetInformationDisplay(true);
        }
    }
    else
    {
        if (PlayerHUD)
        {
            PlayerHUD->SetInformationDisplay(false);

        }
    }
}

void AHappyPlayerController::ShowMainMenuUI(bool bVisible)
{
    if (PlayerHUD)
    {
        if (currentUIState == EUIState::EUIS_Notice) return;

        UpdateAllUI();

        ChangeUiState(EUIState::EUIS_Menu, bVisible);


        if (currentUIState == EUIState::EUIS_Menu)
        {
            PlayerHUD->SetMainMenuDisplay(true, EMenuType::EMT_Map);
        }
        else
        {
            PlayerHUD->SetMainMenuDisplay(false, EMenuType::EMT_Map);
        }

    }
}

void AHappyPlayerController::WarpTravel(EWarpTarget warptarget)
{
    if (!TravelMap.Contains(warptarget)) return;

    FString MapName = TravelMap[warptarget];

    if (MapName.IsEmpty()) return;
    if (PeacFulGameInstance == nullptr) return;

    if (ControlledCharacter && PeaceFulHazardGameMode)
    {
        PlayerHUD->ShowLoadingUI(true);

        bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);


        PeaceFulHazardGameMode->SavePlayerPara(CharacterInventoty, CharacterItemBox, maxBullet, currentBullet, currentHealth, currentBulletType, ControlledCharacter->GetIEquipped(), ControlledCharacter->GetActorLocation(), ControlledCharacter->GetActorRotation());
        PeaceFulHazardGameMode->OpenMap(MapName);
    }

}

void AHappyPlayerController::TutorialShow(ETutorialType tutorialType)
{
    if (PeacFulGameInstance == nullptr) return;
    if (!PeacFulGameInstance->GetTutorialByLanguage().Contains(tutorialType)) return;

    if (!PeacFulGameInstance->checkIsTutorialAlready(tutorialType))
    {
        ShowNoticeUI(true, PeacFulGameInstance->GetTutorialByLanguage()[tutorialType]);
    }

}

bool AHappyPlayerController::ChangeItemInventoryMap(EItemType itemType, int32 count)
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
            return false;
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

    UpdateAllUI();
    return true;

}

bool AHappyPlayerController::ChangeItemInventoryArray(EItemType itemType, int32 count)
{
    int32 Lockindex = GetLockIndex();

    if (count < 0)
    {
        int32 Findindex = 0;
        for (EItemType InventoryitemType : CharacterInventoty.inventoryItems)
        {
            if (InventoryitemType == itemType)
            {
                break;
            }

            Findindex++;
        }

        if (Lockindex <= Findindex)
        {
            return false;
        }

        if (CharacterInventoty.inventoryItemCounts[Findindex] + count >= 0)
        {
            CharacterInventoty.inventoryItemCounts[Findindex] += count;

            if (CharacterInventoty.inventoryItemCounts[Findindex] == 0)
            {
                CharacterInventoty.inventoryItems[Findindex] = EItemType::EIT_None;
            }
        }
        else
        {
            count += CharacterInventoty.inventoryItemCounts[Findindex];

            CharacterInventoty.inventoryItemCounts[Findindex] = 0;
            CharacterInventoty.inventoryItems[Findindex] = EItemType::EIT_None;

            if (!ChangeItemInventoryArray(itemType, count)) return false;
        }

        return true;
    }

    
    if (!CanGetItem(itemType, count)) return false;

    int32 MaxCountUnit = ItemInformation->ItemInformationMap[itemType].itemMaxCount;

    int index = 0; // show last item box, which has same item type
    for (index = 0; index < 15; index++)
    {
        if (CharacterInventoty.inventoryItems[index] == itemType && CharacterInventoty.inventoryItemCounts[index] < MaxCountUnit)
        {
            break;
        }

    }

    // if index == 15, should make new slot to obtain item

    if (index >= 15)
    {
        int32 Emptyindex = 0;
        for (EItemType InventoryitemType : CharacterInventoty.inventoryItems)
        {
            if (InventoryitemType == EItemType::EIT_None)
            {
                break;
            }

            Emptyindex++;
        }

        CharacterInventoty.inventoryItems[Emptyindex] = itemType;
        CharacterInventoty.inventoryItemCounts[Emptyindex] = count;

        if (CharacterInventoty.inventoryItemCounts[Emptyindex] > MaxCountUnit)
        {
            int32 itemOverflow = CharacterInventoty.inventoryItemCounts[Emptyindex] - MaxCountUnit;
            CharacterInventoty.inventoryItemCounts[Emptyindex] = MaxCountUnit;


            if (!ChangeItemInventoryArray(itemType, itemOverflow)) return false;

        }

    }
    else
    {
        int32 FreeSpace = MaxCountUnit - CharacterInventoty.inventoryItemCounts[index];
        FreeSpace = FMath::Clamp(FreeSpace, 0, count);

        CharacterInventoty.inventoryItemCounts[index] += FreeSpace;
        count -= FreeSpace;

        if (count > 0)
        {
            if (!ChangeItemInventoryArray(itemType, count)) return false;
        }

    }

    UpdateAllUI();
    return true;
}

