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


UCLASS()
class PEACEFULHAZARD_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
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


	UPROPERTY(EditAnywhere, Category = "Fire Para")
	float NormalBulletDamage = 5.f;

	UPROPERTY(EditAnywhere, Category = "Fire Para")
	float BigBulletDamage = 10.f;

	EItemType currentItemType = EItemType::EIT_Bullet_Noraml;

public:
	void Fire(FVector CameraPosition, FVector CameraNormalVector);

	void ReloadTrigger();

	void ChangeBulletMode(EItemType itemType = EItemType::EIT_Bullet_Noraml);

};
