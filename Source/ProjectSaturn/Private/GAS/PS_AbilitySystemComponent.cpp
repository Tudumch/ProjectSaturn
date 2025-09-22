// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PS_AbilitySystemComponent.h"

#include "Core/PS_Character.h"
#include "GAS/PS_AttributeSet.h"
#include "GAS/GameplayAbilities/GA_Combo.h"

UPS_AbilitySystemComponent::UPS_AbilitySystemComponent()
{
    BasicAbilities.Add(UGA_Combo::StaticClass());
}

void UPS_AbilitySystemComponent::GiveInitialAbilities()
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    for (const TSubclassOf<UGameplayAbility> & AbilityClass : BasicAbilities)
        GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, -1, nullptr));
    
    for (const TSubclassOf<UGameplayAbility> & AbilityClass : GrantedAbilities)
        GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, -1, nullptr));
}

void UPS_AbilitySystemComponent::ApplyBaseEnergyDrainEffect()
{
    if (!GetOwner()->HasAuthority())
        return;
    
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

    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;
    
    InitAbilityActorInfo(GetOwner(), GetOwner());

    const FGameplayAttribute HealthAttribute = UPS_AttributeSet::GetHealthAttribute();
    GetGameplayAttributeValueChangeDelegate(HealthAttribute).AddUObject(this, &ThisClass::OnAttributeValueChanged);
    const FGameplayAttribute EnergyAttribute = UPS_AttributeSet::GetEnergyAttribute();
    GetGameplayAttributeValueChangeDelegate(EnergyAttribute).AddUObject(this, &ThisClass::OnAttributeValueChanged);

    GiveInitialAbilities();
    ApplyBaseEnergyDrainEffect();
}

void UPS_AbilitySystemComponent::OnAttributeValueChanged(const FOnAttributeChangeData& AttributeChangeData)
{
    if (const APS_Character* Char = Cast<APS_Character>(GetOwner()))
        if (Char->IsDead())
            return;
    
    if (AttributeChangeData.Attribute == UPS_AttributeSet::GetHealthAttribute() && AttributeChangeData.NewValue <= 0.f)
        OnZeroHealthDelegate.Broadcast(GetOwner());
    
    if (AttributeChangeData.Attribute == UPS_AttributeSet::GetEnergyAttribute() && AttributeChangeData.NewValue <= 0.f)
        OnZeroEnergyDelegate.Broadcast(GetOwner());
}