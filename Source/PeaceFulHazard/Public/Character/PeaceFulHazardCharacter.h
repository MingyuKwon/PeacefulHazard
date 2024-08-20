// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PeaceFulHazardCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNowAimingEvent, bool, flag);

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class APlayerHUD;
struct FInputActionValue;
class AHappyPlayerController;
class AWeapon;
class UAnimMontage;
class UBoxComponent;
class AHappyInteractableItem;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class APeaceFulHazardCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* actiontBox;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FNowAimingEvent NowAimingEvent;

	/** Called for movement input */
	bool Move(const FInputActionValue& Value);

	/** Called for looking input */
	bool Look(const FInputActionValue& Value);

	/** Called for Aim input */
	bool AimStart(const FInputActionValue& Value);

	/** Called for Aim input */
	bool AimEnd(const FInputActionValue& Value);

	/** Called for Fire input */
	bool Fire(const FInputActionValue& Value);

	/** Called for Fire input */
	bool ShiftStart(const FInputActionValue& Value);

	/** Called for Fire input */
	bool ShiftEnd(const FInputActionValue& Value);

	/** Called for Fire input */
	bool EquipTrigger(const FInputActionValue& Value);

	/** Called for looking input */
	bool Reload(const FInputActionValue& Value);


	APeaceFulHazardCharacter();

	virtual void Tick(float deltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ReloadEndTrigger();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movemnet Parameter", meta = (AllowPrivateAccess = "true"))
	float AimMoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movemnet Parameter", meta = (AllowPrivateAccess = "true"))
	float DefaultMoveSpeed = 170.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movemnet Parameter", meta = (AllowPrivateAccess = "true"))
	float ShiftMoveSpeed = 300.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseSensitivity = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim Parameter", meta = (AllowPrivateAccess = "true"))
	float MouseAimSensitivity = 0.4f;


	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Bullet Para")
	UAnimMontage* NoAimReloadMontage;

	UPROPERTY(EditAnywhere, Category = "Bullet Para")
	UAnimMontage* AimReloadMontage;

	// this is used for Aiming start and end. using Camera Arm distance lerping (min 0, max 1)
	float AimingPercent = 0.f;

	float DefaultArmLength = 110.f;
	float AimArmLength = 60.f;
	float AimUpArmLength = 20.0f;
	float AimDownArmLength = 60.f;

	FVector DefaultSocketPosition = FVector(0.f, 40.f, 65.f);
	FVector AimSocketPosition = FVector(0.f, 45.f, 50.f);
	FVector AimUpSocketPosition = FVector(0.f, 50.f, 30.f);
	FVector AimDownSocketPosition = FVector(0.f, 50.f, 50.f);

	FRotator DefaultCameraRotation = FRotator(0.f, 0.f, 0.f);
	FRotator AimCameraRotation = FRotator(0.f, -40.f, 0.f);

	void AimingLerp(float deltaTime);
	void AimingPitchLerp(float deltaTime);


	float moveXInput = 0.f;
	float moveYInput = 0.f;
	void SetMoveInputLerp(float aimmoveXInput, float aimmoveYInput);

	void SetMoveSpeed();
	void SetShouldRotate();
	void SetShouldPlayerFollowCamera();

	FTimerHandle LookLock;


protected:

	// To add mapping context
	virtual void BeginPlay();

	APlayerHUD* PlayerHUD;

	AHappyPlayerController* HappyPlayerController;

	AHappyInteractableItem* currentActionableItem;
	TArray<AHappyInteractableItem*> ActionableItems;

	AWeapon* EquipWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Parameter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> PistolClass;

	bool bFireLock = false;
	float PistolFireDelay = 1.f;


	void SetWeaponEquip(bool isEquiped);

	bool bEquiped = false;

	bool bReloading = false;


	bool bShootableAimState = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Action", meta = (AllowPrivateAccess = "true"))
	bool bNowAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Action", meta = (AllowPrivateAccess = "true"))
	bool bNowShifting = false;

	void SetUIUpdateTick();

	void SetCurrentActionItem();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns Is Player is Aiming **/
	UFUNCTION(BlueprintCallable)
	bool GetIsAiming() const;

	UFUNCTION(BlueprintCallable)
	bool GetIEquipped() const { return bEquiped; };

	UFUNCTION(BlueprintCallable)
	bool GetIsShift() const { return bNowShifting; };


	UFUNCTION(BlueprintCallable)
	float GetMoveXInput() const;

	UFUNCTION(BlueprintCallable)
	float GetMoveYInput() const;

	UFUNCTION(BlueprintCallable)
	float GetAimPitch() const;

	UFUNCTION(BlueprintCallable)
	void AddCurrentActionableItem(AHappyInteractableItem* item);

	UFUNCTION(BlueprintCallable)
	void RemoveCurrentActionableItem(AHappyInteractableItem* item);


	UFUNCTION(BlueprintCallable)
	bool GetIsShootable() const { return bShootableAimState; }

};

