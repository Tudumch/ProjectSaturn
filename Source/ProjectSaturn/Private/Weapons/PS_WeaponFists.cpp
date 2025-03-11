// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PS_WeaponFists.h"
#include "KismetTraceUtils.h"

float APS_WeaponFists::StartFire()
{
    GetWorld()->GetTimerManager().SetTimer(CollisionSpawnTimer, this, &ThisClass::SpawnFistsCollision, 0.1f, true);
    return Super::StartFire();
}

void APS_WeaponFists::StopFire()
{
    Super::StopFire();
    bHasCollisionReachedTarget = false;
    GetWorld()->GetTimerManager().ClearTimer(CollisionSpawnTimer);
}

void APS_WeaponFists::SpawnFistsCollision()
{
    if (bHasCollisionReachedTarget || !OwnerMeshComponent) return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    FHitResult HitResult;

    GetWorld()->SweepSingleByChannel(HitResult, OwnerMeshComponent->GetSocketLocation(HandSocketNameRight),
        OwnerMeshComponent->GetSocketLocation
        (HandSocketNameRight), FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(FistCollisionRadius),
        CollisionParams);

    if (!HitResult.GetActor())
        GetWorld()->SweepSingleByChannel(HitResult, OwnerMeshComponent->GetSocketLocation(HandSocketNameLeft),
            OwnerMeshComponent->GetSocketLocation
            (HandSocketNameLeft), FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(FistCollisionRadius),
            CollisionParams);

    if (AActor* HitActor = HitResult.GetActor())
    {
        ApplyDamageToActor(HitActor, DamageAmount);
        bHasCollisionReachedTarget = true;

        if (bDrawDebugSpheres)
        {
            DrawDebugSpheres();
            UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
        }
    }
}

void APS_WeaponFists::DrawDebugSpheres()
{
    if (!OwnerMeshComponent) return;

    FHitResult OutHit;

    DrawDebugSphereTraceSingle(GetWorld(), OwnerMeshComponent->GetSocketLocation(HandSocketNameLeft),
        OwnerMeshComponent->GetSocketLocation
        (HandSocketNameLeft), FistCollisionRadius, EDrawDebugTrace::ForOneFrame, true, OutHit, FLinearColor::Green,
        FLinearColor::Red, 1.f);
    DrawDebugSphereTraceSingle(GetWorld(), OwnerMeshComponent->GetSocketLocation(HandSocketNameRight),
        OwnerMeshComponent->GetSocketLocation
        (HandSocketNameRight), FistCollisionRadius, EDrawDebugTrace::ForOneFrame, true, OutHit, FLinearColor::Green,
        FLinearColor::Red, 1.f);
}

void APS_WeaponFists::BeginPlay()
{
    Super::BeginPlay();
}