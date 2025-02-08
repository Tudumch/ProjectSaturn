// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/PS_Prop_Base.h"
#include "PS_Prop_RechargingCapsule.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API APS_Prop_RechargingCapsule : public APS_Prop_Base
{
	GENERATED_BODY()

public:
    virtual void StartInteract(ACharacter* Character) override;
    virtual void StopInteract() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChargeAmount = 1; // how much charge will the capsule charge per tick
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChargeTick = 1; // how many seconds pass between charging ticks
    UPROPERTY()
    class UPS_EnergyComponent* TargetEnergyComponent;
    UPROPERTY()
    FTimerHandle ChargeTickTimer;

    UFUNCTION(BlueprintCallable)
    void Charge();
};
