// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PS_WeaponBase.h"

#include "Components/PS_HealthComponent.h"
#include "GameFramework/Character.h"

APS_WeaponBase::APS_WeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMeshComponent->SetupAttachment(GetRootComponent());
}

void APS_WeaponBase::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::OnSecondTick);
}

void APS_WeaponBase::OnSecondTick()
{
    if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
        OwnerMeshComponent = Character->GetMesh();
}

void APS_WeaponBase::StartFire()
{
    if (bIsFiring || !FireMontage) return;

    bIsFiring = true;
    GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &ThisClass::StopFire, FireRate);
}

void APS_WeaponBase::StopFire()
{
    bIsFiring = false;
}

void APS_WeaponBase::ApplyDamageToActor(AActor*& Actor, const float Damage)
{
    if (!Actor) return;
    if (UPS_HealthComponent* HealthComp = Actor->GetComponentByClass<UPS_HealthComponent>())
        HealthComp->AddHealth(-Damage);
}