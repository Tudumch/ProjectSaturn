// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PS_WeaponBase.generated.h"

UCLASS()
class PROJECTSATURN_API APS_WeaponBase : public AActor
{
    GENERATED_BODY()

public:
    APS_WeaponBase();

    UFUNCTION(BlueprintCallable)
    virtual float StartFire();
    UFUNCTION(BlueprintCallable)
    virtual void StopFire();

    UFUNCTION(BlueprintPure)
    UAnimMontage* GetFireMontage() const { return FireMontage; } ;

protected:
    UPROPERTY(EditAnywhere)
    float DamageAmount = 30.f;
    UPROPERTY(EditAnywhere)
    float FireRate = 1.f;
    UPROPERTY(EditAnywhere)
    UAnimMontage* FireMontage = nullptr;
    UPROPERTY(EditAnywhere)
    bool bIsFiring = false;

    UPROPERTY()
    FTimerHandle FireRateTimer;

    virtual void BeginPlay() override;

};