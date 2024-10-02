// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MainMenuController.generated.h"

class UInputMappingContext;
class UInputAction;

class AMainMenuGameMode;
class AMainMenuHUD;
/**
 * 
 */
UCLASS()
class PEACEFULHAZARD_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void SetupInputComponent() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RIghtClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftClickAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	AMainMenuGameMode* MainMenuGameMode;
	AMainMenuHUD* MainMenuHUD;

	virtual void BeginPlay() override;

	UFUNCTION()
	void ChangeCameraPosition(EMainMenuType menuType, bool bChangeStart);

	UFUNCTION()
	void SetOnceCheckDisplay(bool bVisible, const FText EnglishText = FText(), const FText KoreanText = FText());


	void RIghtClick(const FInputActionValue& Value);
	void LeftClick(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);

};
