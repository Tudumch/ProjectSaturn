// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PS_WeaponBase.h"
#include "PS_WeaponHitScan.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API APS_WeaponHitScan : public APS_WeaponBase
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BarrelSocketName = "BarrelSocket";
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxRange = 10000.f;

};