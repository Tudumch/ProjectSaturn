// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PS_WeaponBase.h"

// Sets default values
APS_WeaponBase::APS_WeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;

}

void APS_WeaponBase::StopFire()
{
    bIsFiring = false;
}

float APS_WeaponBase::StartFire()
{
    if (bIsFiring || !FireMontage) return 0.f;

    bIsFiring = true;
    GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &ThisClass::StopFire, FireRate);

    return FireMontage->GetPlayLength();
}

void APS_WeaponBase::BeginPlay()
{
    Super::BeginPlay();
}