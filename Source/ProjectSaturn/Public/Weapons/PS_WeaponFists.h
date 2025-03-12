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
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditAnywhere)
    bool bDrawDebugSpheres = true;
    UPROPERTY(EditAnywhere)
    FName HandSocketNameLeft = TEXT("WeaponSocket_L");
    UPROPERTY(EditAnywhere)
    FName HandSocketNameRight = TEXT("WeaponSocket_R");
    UPROPERTY(EditAnywhere)
    float FistCollisionRadius = 25.f;
    UPROPERTY()
    bool bHasCollisionReachedTarget = false; // prevents multiple fist-collision activation

    UPROPERTY()
    FTimerHandle CollisionSpawnTimer;

    UFUNCTION(BlueprintCallable)
    void SpawnFistsCollision();

    UFUNCTION(BlueprintCallable)
    void DrawDebugSpheres();

    virtual void BeginPlay() override;

};