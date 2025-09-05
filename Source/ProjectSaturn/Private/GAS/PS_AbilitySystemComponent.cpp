// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PS_AbilitySystemComponent.h"

void UPS_AbilitySystemComponent::ApplyInitialEffects()
{
    for (const auto EffectClass : InitialEffects)
    {
        FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, MakeEffectContext());
        ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
    }
}