// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/Weapon.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"


// Sets default values
AWeapon::AWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBox"));
    RootComponent = CollisionBox;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
    WeaponMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
    Super::BeginPlay();

}

void AWeapon::Fire(FVector CameraPosition, FVector CameraNormalVector)
{
    if (WeaponMesh == nullptr) return;

    FVector MuzzleLocation = WeaponMesh->GetSocketLocation(FName("Muzzle"));
    FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(FName("Muzzle"));

    FVector Start = CameraPosition;
    FVector End = Start + (CameraNormalVector * 10000.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

    if (bHit)
    {
        if (BulletHitImpact)
        {
            UNiagaraComponent* bulletHit = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                BulletHitImpact,
                HitResult.ImpactPoint,
                HitResult.ImpactNormal.Rotation()
            );

            /*

            UNiagaraComponent* bulletTrail = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                BulletTrail,
                MuzzleLocation
            );

            if (bulletTrail)
            {
                bulletTrail->SetVectorParameter(FName("TrailStart"), MuzzleLocation);
                bulletTrail->SetVectorParameter(FName("TrailEnd"), HitResult.ImpactPoint);
            }

            */



        }

    }


    if (MuzzleEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            MuzzleEffect,
            MuzzleLocation,
            MuzzleRotation
        );
    }

    if (FireMontage)
    {
        UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(FireMontage);
        }
    }
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

