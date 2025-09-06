// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PS_AbilitySystemComponent.h"

#include "GAS/PS_AttributeSet.h"

void UPS_AbilitySystemComponent::ApplyBaseEnergyDrainEffect()
{
    UGameplayEffect* EnergyDrainEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BaseEnergyDrainEffect")));
    EnergyDrainEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;
    EnergyDrainEffect->Period = 1.0f; // Every second 

    bool bAttributeValueValid = false;
    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UPS_AttributeSet::GetEnergyAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    const float EnergyDrainRate = GetGameplayAttributeValue(UPS_AttributeSet::GetEnergyBaseConsumptionRateAttribute(), bAttributeValueValid);
    ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-EnergyDrainRate));
    
    EnergyDrainEffect->Modifiers.Add(ModifierInfo);
    
    ApplyGameplayEffectToSelf(EnergyDrainEffect, 1.0f, MakeEffectContext());
}