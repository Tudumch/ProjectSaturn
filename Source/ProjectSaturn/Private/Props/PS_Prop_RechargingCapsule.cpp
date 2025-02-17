// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/PS_Prop_RechargingCapsule.h"

#include "Components/PS_EnergyComponent.h"
#include "Components/PS_HealthComponent.h"
#include "GameFramework/Character.h"

void APS_Prop_RechargingCapsule::StartInteract(ACharacter* Character)
{
    Super::StartInteract(Character);

    TargetEnergyComponent = Character->GetComponentByClass<UPS_EnergyComponent>();
    TargetHealthComponent = Character->GetComponentByClass<UPS_HealthComponent>();

    GetWorldTimerManager().SetTimer(ChargeTickTimer, this,  &ThisClass::Charge, ChargeTick, true);
}

void APS_Prop_RechargingCapsule::StopInteract()
{
    Super::StopInteract();

    GetWorldTimerManager().ClearTimer(ChargeTickTimer);
}

void APS_Prop_RechargingCapsule::Charge()
{
    if (TargetEnergyComponent) 
        TargetEnergyComponent->AddEnergy(ChargeAmount);

    if (TargetHealthComponent)
        TargetHealthComponent->AddHealth(ChargeAmount);
}