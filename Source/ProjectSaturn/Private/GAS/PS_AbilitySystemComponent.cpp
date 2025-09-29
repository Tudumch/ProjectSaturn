// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PS_AbilitySystemComponent.h"

#include "Core/PS_Character.h"
#include "GAS/PS_AttributeSet.h"
#include "GAS/GameplayAbilities/GA_Combo.h"

UPS_AbilitySystemComponent::UPS_AbilitySystemComponent()
{
    BasicAbilities.Add(EPSAbilityInputID::PrimaryAction, UGA_Combo::StaticClass());
}

void UPS_AbilitySystemComponent::GiveInitialAbilities()
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    for (TPair<EPSAbilityInputID, TSubclassOf<UGameplayAbility>> & AbilityPair : BasicAbilities)
        GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 1, (int8)AbilityPair.Key, nullptr));
    
    for (TPair<EPSAbilityInputID, TSubclassOf<UGameplayAbility>> & AbilityPair : BasicAbilities)
        GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 1, (uint8)AbilityPair.Key, nullptr));
}

void UPS_AbilitySystemComponent::ApplyBaseEnergyDrainEffect()
{
    if (!GetOwner()->HasAuthority())
        return;
    
    static UGameplayEffect* BaseEnergyDrainEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BaseEnergyDrainEffect")));
    BaseEnergyDrainEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;
    BaseEnergyDrainEffect->Period = 1.0f;
    
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(BaseEnergyDrainEffect->GetClass(), 1.0f, MakeEffectContext());
    
    if (SpecHandle.IsValid())
    {
        bool bAttributeValueValid = false;
        const float EnergyDrainRate = GetGameplayAttributeValue(UPS_AttributeSet::GetEnergyBaseConsumptionRateAttribute(), bAttributeValueValid);
        
        FGameplayModifierInfo ModifierInfo;
        ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-EnergyDrainRate));

        if (bAttributeValueValid)
            ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
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