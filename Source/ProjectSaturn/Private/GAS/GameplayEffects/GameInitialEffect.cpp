// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayEffects/GameInitialEffect.h"

#include "GAS/PS_AttributeSet.h"

UGameInitialEffect::UGameInitialEffect()
{
    DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo HealthMod;
    HealthMod.Attribute = UPS_AttributeSet::GetHealthAttribute();
    HealthMod.ModifierOp = EGameplayModOp::Override;
    HealthMod.ModifierMagnitude = FScalableFloat(100.0f); 
        
    FGameplayModifierInfo EnergyMod;
    EnergyMod.Attribute = UPS_AttributeSet::GetEnergyAttribute();
    EnergyMod.ModifierOp = EGameplayModOp::Override;
    EnergyMod.ModifierMagnitude = FScalableFloat(150.0f); 
        
    Modifiers.Add(HealthMod);
    Modifiers.Add(EnergyMod);
}