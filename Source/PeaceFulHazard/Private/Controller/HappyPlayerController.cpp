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

        PeaceFulHazardGameMode->TutorialEvent.AddDynamic(this, &ThisClass::TutorialShow);

        
    }

    InitializeInventory();

    if (PlayerHUD)
    {
        UpdateAllUI();
    }

}


void AHappyPlayerController::InitializeInventory()
{
    CharacterInventoty.ItemLockArray.Init(false, 8);
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
    if (PlayerHUD)
    {
        if (nowPausing)
        {
            PlayerHUD->SetInventoryDisplay(false);
            SetGamePause(false);

        }
        else
        {
            UpdateAllUI();

            currentUIState = EUIState::EUIS_Inventory;
            PlayerHUD->SetInventoryDisplay(true);
            SetGamePause(true);

        }

    }
}


void AHappyPlayerController::GetItem(EItemType itemType, int32 count)
{
    if (PlayerHUD)
    {
        UpdateAllUI();

        currentUIState = EUIState::EUIS_ItemGet;
        PlayerHUD->SetGetItemDisplay(true, itemType, count);
        SetGamePause(true);
    }
}

void AHappyPlayerController::TriggerSituation(EInteractSituationType situtaionType)
{
    if (PlayerHUD)
    {
        UpdateAllUI();

        currentUIState = EUIState::EUIS_ItemGet;
        PlayerHUD->showSituationUI(true, situtaionType);
        SetGamePause(true);
    }
}

void AHappyPlayerController::TriggerItemBox()
{
    if (PlayerHUD)
    {
        if (nowPausing)
        {
            PlayerHUD->SetInventoryDisplay(false);
            SetGamePause(false);

        }
        else
        {
            UpdateAllUI();

            currentUIState = EUIState::EUIS_ItemBox;
            PlayerHUD->SetItemBoxDisplay(true);
            SetGamePause(true);

        }

    }
}

void AHappyPlayerController::TakeDamge(float damage)
{
    currentHealth -= damage;
    currentHealth = FMath::Clamp(currentHealth, 0, 200);

    if (ControlledCharacter)
    {
        if (currentHealth <= 0)
        {
            ControlledCharacter->Death();
        }
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

    if (IsPaused())
    {
        if (PlayerHUD)
        {
            PlayerHUD->OkUIInputTrigger();
        }

        return;
    }

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

    UpdateAllUI();
}

void AHappyPlayerController::RIghtClickStart(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (IsPaused())
    {
        if (PlayerHUD)
        {
            PlayerHUD->BackUIInputTrigger();
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

void AHappyPlayerController::ChangeBullet(const FInputActionValue& Value)
{
    bool bSuccess = false;

    if (ControlledCharacter)
    {
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

    if (!flag)
    {
        currentUIState = EUIState::EUIS_None;
    }
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


void AHappyPlayerController::CloseAllUI()
{
    PlayerHUD->SetGetItemDisplay(false);
    PlayerHUD->SetInventoryDisplay(false);
    PlayerHUD->showSituationUI(false);
    PlayerHUD->SetItemBoxDisplay(false);

    SetGamePause(false);
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

    CloseAllUI();
}

void AHappyPlayerController::SituationInteract(EInteractSituationType situationType)
{
    CloseAllUI();
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
    if (bVisible)
    {
        if (PlayerHUD)
        {
            PauseGame(true);
            PlayerHUD->UpdateNoticeDisplay(noticeText);
            PlayerHUD->SetNoticeDisplay(true);
        }
        
        
    }
    else
    {
        if (PlayerHUD)
        {
            PauseGame(false);
            PlayerHUD->SetNoticeDisplay(false);
        }
    }
}

void AHappyPlayerController::TutorialShow(ETutorialType tutorialType)
{
    if (!TutorialMap.Contains(tutorialType)) return;
    if (PeacFulGameInstance == nullptr) return;

    if (!PeacFulGameInstance->checkIsTutorialAlready(tutorialType))
    {
        ShowNoticeUI(true, TutorialMap[tutorialType]);
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

    int32 Emptyindex = 0;
    for (EItemType InventoryitemType : CharacterInventoty.inventoryItems)
    {
        if (InventoryitemType == EItemType::EIT_None)
        {
            break;
        }

        Emptyindex++;
    }

    if (Lockindex <= Emptyindex)
    {
        return false;
    }

    CharacterInventoty.inventoryItems[Emptyindex] = itemType;
    CharacterInventoty.inventoryItemCounts[Emptyindex] = count;

    if (CharacterInventoty.inventoryItemCounts[Emptyindex] > ItemInformation->ItemInformationMap[itemType].itemMaxCount)
    {
        if (!ChangeItemInventoryArray(itemType, CharacterInventoty.inventoryItemCounts[Emptyindex] - ItemInformation->ItemInformationMap[itemType].itemMaxCount)) return false;

        CharacterInventoty.inventoryItemCounts[Emptyindex] = ItemInformation->ItemInformationMap[itemType].itemMaxCount;
    }


    return true;
}

