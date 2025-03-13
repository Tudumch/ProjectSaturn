// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PS_WeaponHitScan.h"

void APS_WeaponHitScan::StartFire()
{
    Super::StartFire();

    const FTransform BarrelSocketTransform = SkeletalMeshComponent->GetSocketTransform(BarrelSocketName);
    const FVector StartLocation = BarrelSocketTransform.GetLocation();
    const FVector EndLocation = StartLocation + BarrelSocketTransform.GetRotation().GetForwardVector() * MaxRange;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_WorldDynamic, CollisionParams);
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.f);

    if (AActor* HitActor = HitResult.GetActor())
        ApplyDamageToActor(HitActor, DamageAmount);
}