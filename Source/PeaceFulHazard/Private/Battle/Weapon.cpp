// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/Weapon.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Battle/PistolShell.h"
#include "Materials/MaterialInstance.h"
#include "Enemy/EnemyBase.h"
#include "Engine/DamageEvents.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "GameMode/PeaceFulHazardGameMode.h"

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
    PeaceFulHazardGameMode = Cast<APeaceFulHazardGameMode>(UGameplayStatics::GetGameMode(this));

    if (PeaceFulHazardGameMode)
    {
        switch (PeaceFulHazardGameMode->GetDifficulty())
        {
        case EDifficulty::ED_Easy:
            NormalBulletDamage = Easy_NormalBulletDamage;
            BigBulletDamage = Easy_BigBulletDamage;
            break;

        case EDifficulty::ED_Normal:
            NormalBulletDamage = Normal_NormalBulletDamage;
            BigBulletDamage = Normal_BigBulletDamage;
            break;

        case EDifficulty::ED_Hard:
            NormalBulletDamage = Hard_NormalBulletDamage;
            BigBulletDamage = Hard_BigBulletDamage;
            break;
        }

    }
}

void AWeapon::ChangeBulletMode(EItemType itemType)
{
    currentItemType = itemType;

    if (itemType == EItemType::EIT_Bullet_Noraml)
    {
        if (NormalMaterial)
        {
            WeaponMesh->SetMaterial(0, NormalMaterial);
        }
    }
    else
    {
       
        if (BigMaterial)
        {
            WeaponMesh->SetMaterial(0, BigMaterial);
        }
    }
}

void AWeapon::ShootAtEnemy(AActor* TargetActor, FVector HitLocation, FVector ShotDirection, FName BoneName, float damagepercent)
{
    float DamageAmount = 0.f;  

    if (currentItemType == EItemType::EIT_Bullet_Big)
    {
        DamageAmount = BigBulletDamage;
    }
    else
    {
        DamageAmount = NormalBulletDamage;
    }

    DamageAmount *= damagepercent;

    FPointDamageEvent PointDamageEvent;
    PointDamageEvent.Damage = DamageAmount;
    PointDamageEvent.ShotDirection = ShotDirection;

    PointDamageEvent.HitInfo.Location = HitLocation;
    PointDamageEvent.HitInfo.BoneName = BoneName;

    AActor* OwnerActor = GetOwner(); 
    AController* OwnerController = nullptr;

    if (OwnerActor)
    {
        APawn* OwnerPawn = Cast<APawn>(OwnerActor);
        if (OwnerPawn)
        {
            OwnerController = OwnerPawn->GetController();  
        }
    }

    if (OwnerController)
    {
        UGameplayStatics::ApplyPointDamage(TargetActor, DamageAmount, ShotDirection, PointDamageEvent.HitInfo, OwnerController, this, UDamageType::StaticClass());

        UAISense_Damage::ReportDamageEvent(
            GetWorld(),
            TargetActor,  // Damaged actor
            OwnerActor,  // Instigator (damage causer)
            DamageAmount,  // Damage amount
            GetActorLocation(),  // Event location
            HitLocation  // Hit location (optional)
        );
    }
}

void AWeapon::Fire(FVector CameraPosition, FVector CameraNormalVector, float damagepercent, float aimFocusLerpValue)
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
        FVector SphereCenter = HitResult.ImpactPoint;
        float SphereRadius = aimFocusLerpValue * 45.f;

        FVector RandomDirection = FMath::VRand(); // 랜덤 방향
        FVector RandomPointInSphere = SphereCenter + RandomDirection * FMath::FRandRange(0.f, SphereRadius);

        //DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, false, 3.f);

        bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, Start + (RandomPointInSphere - Start).GetSafeNormal() * 10000.0f, ECC_Visibility, Params);
    }

    if (PeaceFulHazardGameMode)
    {
        switch (currentItemType)
        {
        case EItemType::EIT_Bullet_Noraml :
            PeaceFulHazardGameMode->PlaySoundInGameplay(NormalFireSound, GetActorLocation(), 1.f);
            break;

        case EItemType::EIT_Bullet_Big :
            PeaceFulHazardGameMode->PlaySoundInGameplay(BigFireSound, GetActorLocation(), 1.5f);
            break;
        }
    }

    UAISense_Hearing::ReportNoiseEvent(
        GetWorld(),
        GetActorLocation(),  // �Ҹ� �߻� ��ġ (���� ��ġ)
        1.0f,  // �Ҹ��� ũ�� (Loudness), ��Ȳ�� �°� ���� ����
        GetOwner(),  // �Ҹ��� �߻���Ų ���� (���� �߻��� ��ü)
        0.0f,  // �Ҹ� ���� �ð� (0�̸� ��� �Ҹ� �̺�Ʈ �߻� �� ����)
        TEXT("WeaponNoise")  // �Ҹ��� Ÿ�� (���ϴ� ��� ����� �� �ִ� �±�)
    );

    if (bHit)
    {
        FVector SocketLocation = WeaponMesh->GetSocketLocation(FName("ShellSocket"));
        FRotator SocketRotation = WeaponMesh->GetSocketRotation(FName("ShellSocket"));

        AEnemyBase* hitEnemy;
        if (HitResult.bBlockingHit)
        {
            hitEnemy = Cast<AEnemyBase>(HitResult.GetActor());

            if (hitEnemy)
            {
                if (HitResult.GetComponent() == hitEnemy->HeadBox)
                {
                    ShootAtEnemy(hitEnemy, HitResult.ImpactPoint, (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal(), FName("Head"), damagepercent);
                }
                else
                {
                    ShootAtEnemy(hitEnemy, HitResult.ImpactPoint, (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal(), FName("Body"), damagepercent);

                }
            }
        }

        if (currentItemType == EItemType::EIT_Bullet_Big)
        {
            if (BigBulletHitImpact)
            {
                UNiagaraComponent* bulletHit = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    GetWorld(),
                    BigBulletHitImpact,
                    HitResult.ImpactPoint,
                    HitResult.ImpactNormal.Rotation()
                );
            }
        }
        else
        {
            if (BulletHitImpact)
            {
                UNiagaraComponent* bulletHit = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    GetWorld(),
                    BulletHitImpact,
                    HitResult.ImpactPoint,
                    HitResult.ImpactNormal.Rotation()
                );
            }
        }


        if (ShellClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            APistolShell* shell = GetWorld()->SpawnActor<APistolShell>(ShellClass, SpawnParams);

            if (shell)
            {
                shell->SetActorLocation(SocketLocation);
                shell->SetActorRotation(SocketRotation);

                shell->SetLifeSpan(3.f);

                UStaticMeshComponent* ShellMesh = Cast<UStaticMeshComponent>(shell->GetComponentByClass(UStaticMeshComponent::StaticClass()));
                if (ShellMesh)
                {
                    FVector ImpulseDirection = FVector(0.0f, 1.0f, 0.0f);  
                    float ImpulseStrength = 70.0f;

                    ShellMesh->AddImpulse(ImpulseDirection * ImpulseStrength, NAME_None, true);
                }
            }
        }

    }

    if (currentItemType == EItemType::EIT_Bullet_Big)
    {
        if (BigMuzzleEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                BigMuzzleEffect,
                MuzzleLocation,
                MuzzleRotation
            );
        }
    }
    else
    {
        if (MuzzleEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                MuzzleEffect,
                MuzzleLocation,
                MuzzleRotation
            );
        }
    }


    if (FireMontage)
    {
        UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(FireMontage);
        }
    }



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

void AWeapon::ReloadTrigger()
{
    if (ReloadMontage)
    {
        UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(ReloadMontage);
        }
    }
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

