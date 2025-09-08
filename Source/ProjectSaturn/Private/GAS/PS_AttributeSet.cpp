// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PS_AttributeSet.h"

#include "Net/UnrealNetwork.h"

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

void UPS_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION_NOTIFY(UPS_AttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPS_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPS_AttributeSet, Energy, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPS_AttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
}

void UPS_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_AttributeSet, Health, OldValue);
}
inline void UPS_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_AttributeSet, MaxHealth, OldValue);
}

inline void UPS_AttributeSet::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_AttributeSet, Energy, OldValue);
}

inline void UPS_AttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPS_AttributeSet, MaxEnergy, OldValue);
}
