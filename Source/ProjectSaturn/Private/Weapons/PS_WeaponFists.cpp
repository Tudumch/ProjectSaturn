// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PS_WeaponFists.h"

#include "KismetTraceUtils.h"
#include "GameFramework/Character.h"

float APS_WeaponFists::StartFire()
{
    GetWorld()->GetTimerManager().SetTimer(CollisionSpawnTimer, this, &ThisClass::OnCollisionSpawn, 0.1f, true);
    return Super::StartFire();
}

void APS_WeaponFists::StopFire()
{
    Super::StopFire();
    GetWorld()->GetTimerManager().ClearTimer(CollisionSpawnTimer);
}

void APS_WeaponFists::OnCollisionSpawn()
{
    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    const UMeshComponent* Mesh = Character->GetMesh();
    if (!Mesh) return;

    // TODO: need to replace the debug trace with a real collision detector.
    FHitResult OutHit;
    DrawDebugSphereTraceSingle(GetWorld(), Mesh->GetSocketLocation(HandSocketLeftName), Mesh->GetSocketLocation
        (HandSocketLeftName), FistCollisionRadius, EDrawDebugTrace::ForOneFrame, true, OutHit, FLinearColor::Green,
        FLinearColor::Red, 1.f);
    DrawDebugSphereTraceSingle(GetWorld(), Mesh->GetSocketLocation(HandSocketRightName), Mesh->GetSocketLocation
        (HandSocketRightName), FistCollisionRadius, EDrawDebugTrace::ForOneFrame, true, OutHit, FLinearColor::Green,
        FLinearColor::Red, 1.f);
}

void APS_WeaponFists::BeginPlay()
{
    Super::BeginPlay();
}