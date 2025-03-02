// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PS_WeaponBase.h"
#include "PS_WeaponFists.generated.h"

UCLASS()
class PROJECTSATURN_API APS_WeaponFists : public APS_WeaponBase
{
    GENERATED_BODY()

public:
    virtual float StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditAnywhere)
    FName HandSocketLeftName = TEXT("WeaponSocket_L");
    UPROPERTY(EditAnywhere)
    FName HandSocketRightName = TEXT("WeaponSocket_R");
    UPROPERTY(EditAnywhere)
    float FistCollisionRadius = 25.f;

    UPROPERTY()
    FTimerHandle CollisionSpawnTimer;

    UFUNCTION(BlueprintCallable)
    void OnCollisionSpawn();

    virtual void BeginPlay() override;

};