// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PS_AttributeSet.h"

UPS_AttributeSet::UPS_AttributeSet()
{
    MaxHealth.SetBaseValue(100.0f);
    MaxHealth.SetCurrentValue(100.0f);
    Health.SetBaseValue(MaxHealth.GetCurrentValue());
    Health.SetCurrentValue(MaxHealth.GetCurrentValue());
    
    MaxEnergy.SetBaseValue(150.0f);
    MaxEnergy.SetCurrentValue(150.0f);
    Energy.SetBaseValue(MaxEnergy.GetCurrentValue());
    Energy.SetCurrentValue(MaxEnergy.GetCurrentValue());
    EnergyBaseConsumptionRate.SetBaseValue(0.5);
    EnergyBaseConsumptionRate.SetCurrentValue(0.5);
}

void UPS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    if (Attribute == GetHealthAttribute())
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    if (Attribute == GetEnergyAttribute())
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxEnergy());
}
