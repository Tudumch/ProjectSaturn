// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Library/PS_Enums.h"
#include "PS_WeaponBase.generated.h"

UCLASS()
class PROJECTSATURN_API APS_WeaponBase : public AActor
{
    GENERATED_BODY()

public:
    APS_WeaponBase();

    UFUNCTION(BlueprintCallable)
    virtual void StartFire();
    UFUNCTION(BlueprintCallable)
    virtual void StopFire();

    UFUNCTION(BlueprintPure)
    bool IsFiring() const { return bIsFiring; } ;
    UFUNCTION(BlueprintPure)
    float GetFireRate() const { return FireRate; } ;
    UFUNCTION(BlueprintPure)
    UAnimMontage* GetFireMontage() const { return FireMontage; } ;
    UFUNCTION(BlueprintPure)
    EWeaponType GetWeaponType() const { return WeaponType; } ;

protected:
    UPROPERTY(EditAnywhere)
    float DamageAmount = 30.f;
    UPROPERTY(EditAnywhere)
    float FireRate = 1.f;
    UPROPERTY(EditDefaultsOnly)
    EWeaponType WeaponType = EWeaponType::BareHands;
    UPROPERTY(EditAnywhere)
    UAnimMontage* FireMontage = nullptr;
    UPROPERTY(EditAnywhere)
    bool bIsFiring = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USceneComponent* Root;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USkeletalMeshComponent* SkeletalMeshComponent = nullptr;
    UPROPERTY()
    UMeshComponent* OwnerMeshComponent = nullptr;

    UPROPERTY()
    FTimerHandle FireRateTimer;

    virtual void BeginPlay() override;

    // Finds Actor's Health-component and applies damage to it.
    UFUNCTION(Blueprintable)
    virtual void ApplyDamageToActor(AActor* & Actor, const float Damage);

    // Calls on second tick after game start.
    UFUNCTION(BlueprintCallable)
    virtual void OnSecondTick();

};