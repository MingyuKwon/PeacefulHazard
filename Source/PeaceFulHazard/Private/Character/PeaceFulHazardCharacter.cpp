// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PeaceFulHazardCharacter.h"
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
#include "Components/BoxComponent.h"
#include "Item/HappyInteractableItem.h"
#include "GameMode/PeaceFulHazardGameMode.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AHappyHazardCharacter

APeaceFulHazardCharacter::APeaceFulHazardCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SocketOffset = FVector(0.f, 30.f, 75.f);
	CameraBoom->TargetArmLength = 120.f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(RootComponent); 

	actiontBox = CreateDefaultSubobject<UBoxComponent>(TEXT("actiontBox"));
	actiontBox->SetupAttachment(RootComponent);

	

}

void APeaceFulHazardCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (GEngine)
	{
		FString text = FString::Printf(TEXT("x : %f"), moveXInput);
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Blue, text);
	}

	if (GEngine)
	{
		FString text = FString::Printf(TEXT("y : %f"), moveYInput);
		GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Red, text);
	}

	if (GetCharacterMovement()->Velocity.Length() <= 0)
	{
		SetMoveInputLerp(0.f, 0.f);
	}


	AimingLerp(deltaTime);
	AimingPitchLerp(deltaTime);

	SetUIUpdateTick();

	SetMoveSpeed();
	SetShouldRotate();
	SetShouldPlayerFollowCamera();

}

void APeaceFulHazardCharacter::AimingLerp(float deltaTime)
{
	if (GetIsAiming())
	{
		AimingPercent += deltaTime * 5;

	}
	else
	{
		AimingPercent -= deltaTime * 5;

	}

	AimingPercent = FMath::Clamp(AimingPercent, 0.f, 1.f);
	bShootableAimState = (AimingPercent >= 0.99f);


	float LerpArmLength = FMath::Lerp(DefaultArmLength, AimArmLength, AimingPercent);
	FVector LerpSocketPosition = FMath::Lerp(DefaultSocketPosition, AimSocketPosition, AimingPercent);

	CameraBoom->TargetArmLength = LerpArmLength;
	CameraBoom->SocketOffset = LerpSocketPosition;


}

void APeaceFulHazardCharacter::AimingPitchLerp(float deltaTime)
{
	if (!GetIsAiming()) return;

	float Pitch = GetAimPitch();

	Pitch = Pitch / 90;

	if (Pitch >= 0)
	{
		float LerpArmLength = FMath::Lerp(AimArmLength, AimUpArmLength, Pitch);
		CameraBoom->TargetArmLength = LerpArmLength;

		FVector LerpSocketPosition = FMath::Lerp(AimSocketPosition, AimUpSocketPosition, Pitch);
		CameraBoom->SocketOffset = LerpSocketPosition;

	}
	else
	{
		Pitch = FMath::Abs(Pitch);
		float LerpArmLength = FMath::Lerp(AimArmLength, AimDownArmLength, Pitch);
		CameraBoom->TargetArmLength = LerpArmLength;

		FVector LerpSocketPosition = FMath::Lerp(AimSocketPosition, AimDownSocketPosition, Pitch);
		CameraBoom->SocketOffset = LerpSocketPosition;

	}
}

void APeaceFulHazardCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetController())
	{
		HappyPlayerController = Cast<AHappyPlayerController>(GetController());
	}

	if (HappyPlayerController)
	{
		PlayerHUD = Cast<APlayerHUD>(HappyPlayerController->GetHUD());
	}

	PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(this));

}



void APeaceFulHazardCharacter::SetWeaponEquip(bool isEquiped)
{

}

void APeaceFulHazardCharacter::SetUIUpdateTick()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetAimDisplay(GetIsAiming());
	}

}

bool APeaceFulHazardCharacter::GetIsAiming() const
{
	return bNowAiming && !GetCharacterMovement()->IsFalling() && !bReloading;
}

float APeaceFulHazardCharacter::GetMoveXInput() const
{
	return moveXInput;
}

float APeaceFulHazardCharacter::GetMoveYInput() const
{
	return moveYInput;
}

float APeaceFulHazardCharacter::GetAimPitch() const
{
	float Pitch = 0.f;

	if (Controller)
	{
		Pitch = Controller->GetControlRotation().Pitch;
		if (Pitch >= 180)
		{
			Pitch -= 360;
		}
		Pitch = FMath::Clamp(Pitch, -90.f, 90.f);

	}

	return Pitch;
}

void APeaceFulHazardCharacter::AddCurrentActionableItem(AHappyInteractableItem* item)
{
	if (item == nullptr) return;

	if (!ActionableItems.Contains(item))
	{
		ActionableItems.Add(item);
	}

	SetCurrentActionItem();
}

void APeaceFulHazardCharacter::RemoveCurrentActionableItem(AHappyInteractableItem* item)
{
	if (item == nullptr) return;

	if (ActionableItems.Contains(item))
	{
		ActionableItems.Remove(item);
	}

	if (ActionableItems.Num() <= 0)
	{
		currentActionableItem = nullptr;
		item->SetbActionable(false);
	}
	else
	{
		SetCurrentActionItem();
	}
}


void APeaceFulHazardCharacter::SetCurrentActionItem()
{
	if (ActionableItems.Num() <= 0)
	{
		if (currentActionableItem)
		{
			currentActionableItem->SetbActionable(false);
		}

		currentActionableItem = nullptr;
		return;
	}

	float MinDistance = FLT_MAX;
	AHappyInteractableItem* ClosestItem = nullptr;

	FVector PlayerLocation = GetActorLocation();

	for (AHappyInteractableItem* Item : ActionableItems)
	{
		if (Item)
		{
			float Distance = FVector::Dist(PlayerLocation, Item->GetActorLocation());

			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestItem = Item;
			}
		}
	}

	if (currentActionableItem)
	{
		currentActionableItem->SetbActionable(false);
	}

	currentActionableItem = ClosestItem;

	if (currentActionableItem)
	{
		currentActionableItem->SetbActionable(true);
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void APeaceFulHazardCharacter::SetMoveInputLerp(float aimmoveXInput, float aimmoveYInput)
{
	moveXInput = FMath::Lerp(moveXInput, aimmoveXInput, 0.023f);
	moveYInput = FMath::Lerp(moveYInput, aimmoveYInput, 0.023f);
}

void APeaceFulHazardCharacter::SetMoveSpeed()
{
	float Speed = DefaultMoveSpeed;

	if (GetIsAiming())
	{
		Speed = AimMoveSpeed;
	}
	else if (bNowShifting)
	{
		Speed = ShiftMoveSpeed;

		if (!bEquiped)
		{
			Speed += 20.f;
		}

	}

	GetCharacterMovement()->MaxWalkSpeed = Speed;

}

void APeaceFulHazardCharacter::SetShouldRotate()
{
	if (GetIsAiming())
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		return;
	}



	if (bEquiped && !bNowShifting)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		return;
	}


	GetCharacterMovement()->bOrientRotationToMovement = true;

}

void APeaceFulHazardCharacter::SetShouldPlayerFollowCamera()
{
    if (GetIsAiming())
    {
        FRotator CurrentRotation = GetActorRotation();
        FRotator TargetRotation = Controller->GetControlRotation();
        TargetRotation.Pitch = 0;

        SetActorRotation(TargetRotation);
    }

    if (bEquiped)
    {
        if (!bNowShifting && GetCharacterMovement()->Velocity.Length() > 0)
        {
            FRotator CurrentRotation = GetActorRotation();
            FRotator TargetRotation = Controller->GetControlRotation();
            TargetRotation.Pitch = 0;

            SetActorRotation(TargetRotation);
        }
    }
}


bool APeaceFulHazardCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementVector = MovementVector.GetSafeNormal();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);


		SetMoveInputLerp(MovementVector.X, MovementVector.Y);

		return true;

	}

	return false;

}

bool APeaceFulHazardCharacter::Look(const FInputActionValue& Value)
{

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetIsAiming())
	{
		LookAxisVector = LookAxisVector * MouseAimSensitivity;
	}
	else
	{
		LookAxisVector = LookAxisVector * MouseSensitivity;
	}

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		return true;

	}

	return false;

}

bool APeaceFulHazardCharacter::AimStart(const FInputActionValue& Value)
{
	if (!bEquiped) return false;

	bNowAiming = true;
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->NowAimingEvent.Broadcast(true);
	}
	

	return true;
}

bool APeaceFulHazardCharacter::AimEnd(const FInputActionValue& Value)
{
	bNowAiming = false;
	if (PeaceFulHazardGameMode)
	{
		PeaceFulHazardGameMode->NowAimingEvent.Broadcast(false);
	}
	return true;

}

bool APeaceFulHazardCharacter::Fire(const FInputActionValue& Value)
{
	if (!bShootableAimState) return false;
	if (bFireLock) return false;

	bool bNormalFire = true;

	bFireLock = true;
	if (HappyPlayerController)
	{
		bNormalFire = HappyPlayerController->GetcurrentBulletType() == EItemType::EIT_Bullet_Noraml;
	}

	if (EquipWeapon)
	{
		if (HappyPlayerController == nullptr) return false;

		int32 ViewportSizeX, ViewportSizeY;
		HappyPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		FVector2D ScreenLocation(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f);

		FVector WorldLocation, WorldDirection;
		HappyPlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection);


		EquipWeapon->Fire(WorldLocation, WorldDirection);
	}

	if (bNormalFire)
	{
		if (FireMontage && GetMesh())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(FireMontage);
			}
		}
	}
	else
	{
		if (StrongFireMontage && GetMesh())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(StrongFireMontage);
			}
		}
	}

	FTimerHandle FireDelayHandle;
	GetWorld()->GetTimerManager().SetTimer(FireDelayHandle, [this]()
		{
			bFireLock = false;
		}, bNormalFire ? PistolFireDelay : PistolPowerFireDelay, false);

	return true;
}

bool APeaceFulHazardCharacter::TriggerInteract()
{
	if (currentActionableItem == nullptr) return false;

	currentActionableItem->InteractWithPlayer(this);

	return true;
}


bool APeaceFulHazardCharacter::ShiftStart(const FInputActionValue& Value)
{
	bNowShifting = true;
	return true;

}

bool APeaceFulHazardCharacter::ShiftEnd(const FInputActionValue& Value)
{
	bNowShifting = false;

	return true;

}

bool APeaceFulHazardCharacter::EquipTrigger(const FInputActionValue& Value)
{
	if (GetIsAiming()) return false;
	if (bReloading) return false;

	bEquiped = !bEquiped;

	if (EquipWeapon)
	{
		EquipWeapon->Destroy();
		EquipWeapon = nullptr;
	}

	if (bEquiped)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		if (PistolClass)
		{
			EquipWeapon = GetWorld()->SpawnActor<AWeapon>(PistolClass, SpawnParams);
		}

		if (EquipWeapon)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			EquipWeapon->AttachToComponent(GetMesh(), AttachmentRules, FName("PistolSocket"));
		}
	}

	return true;
}

bool APeaceFulHazardCharacter::Reload(const FInputActionValue& Value)
{
	if (!bEquiped) return false;
	if (bReloading) return false;

	bReloading = true;
	
	if (GetMesh())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			if (AimReloadMontage)
			{
				AnimInstance->Montage_Play(AimReloadMontage);
			}

		}

		if (EquipWeapon)
		{
			EquipWeapon->ReloadTrigger();
		}


		return true;
	}


	return false;
}

void APeaceFulHazardCharacter::ReloadEndTrigger()
{
	bReloading = false;

	if (HappyPlayerController)
	{
		HappyPlayerController->SetBulletCount(false);
	}
}

bool APeaceFulHazardCharacter::ChangeBullet()
{
	if (!bEquiped) return false;
	if (bReloading) return false;

	bReloading = true;

	if (GetMesh())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			if (AimBulletChangeMontage)
			{
				AnimInstance->Montage_Play(AimBulletChangeMontage);
			}

		}

		if (EquipWeapon)
		{
			EquipWeapon->ReloadTrigger();
		}

		return true;
	}


	return false;
}

void APeaceFulHazardCharacter::ChangeBulletEndTrigger()
{
	bReloading = false;

	if (EquipWeapon && HappyPlayerController)
	{
		HappyPlayerController->SetBulletChangeCount();
		EquipWeapon->ChangeBulletMode(HappyPlayerController->GetcurrentBulletType());

	}
}

