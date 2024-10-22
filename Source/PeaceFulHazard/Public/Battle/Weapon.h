// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemInformation.h"
#include "Weapon.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class UAnimMontage;
class APistolShell;
class USoundBase;
class APeaceFulHazardGameMode;

UCLASS()
class PEACEFULHAZARD_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* NormalFireSound;

	UPROPERTY(EditAnywhere, Category = "Sound Para")
	USoundBase* BigFireSound;

	APeaceFulHazardGameMode* PeaceFulHazardGameMode;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* BulletHitImpact;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* BigMuzzleEffect;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* BigBulletHitImpact;


	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UNiagaraSystem* BulletTrail;



	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	TSubclassOf<APistolShell> ShellClass;

	UPROPERTY(EditDefaultsOnly, Category = "Material Parameter")
	UMaterialInstance* NormalMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Material Parameter")
	UMaterialInstance* BigMaterial;


	float NormalBulletDamage = 10.f;
	float BigBulletDamage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	float Easy_NormalBulletDamage = 40.f;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	float Normal_NormalBulletDamage = 30.f;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	float Hard_NormalBulletDamage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	float Easy_BigBulletDamage = 80.f;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	float Normal_BigBulletDamage = 60.f;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	float Hard_BigBulletDamage = 40.f;

	EItemType currentItemType = EItemType::EIT_Bullet_Noraml;

	void ShootAtEnemy(AActor* TargetActor, FVector HitLocation, FVector ShotDirection, FName BoneName, float damagepercent);

public:
	void Fire(FVector CameraPosition, FVector CameraNormalVector, float damagepercent, float aimFocusLerpValue);

	void ReloadTrigger();

	void ChangeBulletMode(EItemType itemType = EItemType::EIT_Bullet_Noraml);

};
