// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PS_WeaponComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTSATURN_API UPS_WeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPS_WeaponComponent();

    UFUNCTION(BlueprintCallable)
    void AttackBaseOnPressed();
    UFUNCTION(BlueprintCallable)
    void AttackBaseOnReleased();

protected:
    UPROPERTY(BlueprintReadWrite)
    bool bIsAttacking = false;

    UPROPERTY(BlueprintReadWrite)
    class APS_WeaponBase* ActiveWeapon = nullptr;
    UPROPERTY(BlueprintReadWrite)
    class UPS_AnimInstance* AnimInstance = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponSocketRightName = TEXT("WeaponSocket_R");
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponSocketLeftName = TEXT("WeaponSocket_L");

    UPROPERTY(EditDefaultsOnly, Category= "Weapon")
    TSubclassOf<APS_WeaponBase> InitialWeaponClass;

    FTimerHandle FireCyclingTimerHandle;
    float MinimalDelayBetweenShots = 0.1f; // in seconds

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    virtual void Fire();
};