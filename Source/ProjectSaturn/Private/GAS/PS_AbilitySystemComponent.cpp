// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PS_AbilitySystemComponent.h"

#include "GAS/PS_AttributeSet.h"

void UPS_AbilitySystemComponent::ApplyBaseEnergyDrainEffect()
{
    UGameplayEffect* EnergyDrainEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BaseEnergyDrainEffect")));
    EnergyDrainEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;
    EnergyDrainEffect->Period = 1.0f; // Every second 

    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UPS_AttributeSet::GetEnergyAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    bool bAttributeValueValid = false;
    const float EnergyDrainRate = GetGameplayAttributeValue(UPS_AttributeSet::GetEnergyBaseConsumptionRateAttribute(), bAttributeValueValid);
    ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-EnergyDrainRate));
    
    EnergyDrainEffect->Modifiers.Add(ModifierInfo);
    
    ApplyGameplayEffectToSelf(EnergyDrainEffect, 1.0f, MakeEffectContext());
}

void UPS_AbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();

    const FGameplayAttribute HealthAttribute = UPS_AttributeSet::GetHealthAttribute();
    GetGameplayAttributeValueChangeDelegate(HealthAttribute).AddUObject(this, &ThisClass::OnAttributeValueChanged);
    const FGameplayAttribute EnergyAttribute = UPS_AttributeSet::GetEnergyAttribute();
    GetGameplayAttributeValueChangeDelegate(EnergyAttribute).AddUObject(this, &ThisClass::OnAttributeValueChanged);
}

void UPS_AbilitySystemComponent::OnAttributeValueChanged(const FOnAttributeChangeData& AttributeChangeData)
{
    if (AttributeChangeData.Attribute == UPS_AttributeSet::GetHealthAttribute() && AttributeChangeData.NewValue <= 0.f)
        OnZeroHealthDelegate.Broadcast(GetOwner());
    
    if (AttributeChangeData.Attribute == UPS_AttributeSet::GetEnergyAttribute() && AttributeChangeData.NewValue <= 0.f)
        OnZeroEnergyDelegate.Broadcast(GetOwner());
}