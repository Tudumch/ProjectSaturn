// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayEffects/WeaponEffects/BareHandsAttackEffect.h"

#include "GAS/PS_AttributeSet.h"

UBareHandsAttackEffect::UBareHandsAttackEffect()
{
    DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UPS_AttributeSet::GetHealthAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FScalableFloat(-50.0f); 
        
    Modifiers.Add(ModifierInfo);
}