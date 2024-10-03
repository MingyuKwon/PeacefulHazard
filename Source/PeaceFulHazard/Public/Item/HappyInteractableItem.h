// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeaceFulHazard/PeaceFulHazard.h"
#include "HappyInteractableItem.generated.h"

class UBoxComponent;
class UWidgetComponent;
class UCanInteractWidget;
class APeaceFulHazardGameMode;
class APeaceFulHazardCharacter;

UCLASS()
class PEACEFULHAZARD_API AHappyInteractableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHappyInteractableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	UTexture2D* DoorInteractkeyImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials Para")
	UMaterialInterface* DisableMaterial;


	FString GetKeyName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	FString DoorInteractkeyNameEnglish = FString("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Parameter")
	FString DoorInteractkeyNameKorean = FString("");


	UCanInteractWidget* CanInteractWidget;

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	APeaceFulHazardCharacter* OverlappingCharacter;

	bool bIntearctable = false;
	bool bActionable = false;

	bool bAlreadyInteract = false;

	bool bInteractWithUI = false;

	virtual void AfterInteraction();

	UFUNCTION()
	void UICloseAllInteractWithItemOver();

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;

	bool bCanInteractable = true;

	UFUNCTION()
	void AImVisibleUI(bool flag);

	void DestroyItem();

	UFUNCTION()
	void MapStartCallBack();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetbActionable(bool flag);

	UFUNCTION()
	virtual void InteractWithPlayer(APeaceFulHazardCharacter* character);

};
