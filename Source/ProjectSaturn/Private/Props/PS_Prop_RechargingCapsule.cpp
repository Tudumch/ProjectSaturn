// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/PS_Prop_RechargingCapsule.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GAS/PS_AttributeSet.h"

void APS_Prop_RechargingCapsule::StartInteract(ACharacter* Character)
{
    Super::StartInteract(Character);

    if (UAbilitySystemComponent* AbilitySystemComponent = Character->GetComponentByClass<UAbilitySystemComponent>())
    {
        UGameplayEffect* RechargingCapsuleEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("RechargingCapsuleEffect")));
        RechargingCapsuleEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;
        RechargingCapsuleEffect->Period = ChargeTick;

        FGameplayModifierInfo EnergyModifierInfo;
        EnergyModifierInfo.Attribute = UPS_AttributeSet::GetEnergyAttribute();
        EnergyModifierInfo.ModifierOp = EGameplayModOp::Additive;
        EnergyModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(ChargeAmount));

        FGameplayModifierInfo HealthModifierInfo;
        HealthModifierInfo.Attribute = UPS_AttributeSet::GetHealthAttribute();
        HealthModifierInfo.ModifierOp = EGameplayModOp::Additive;
        HealthModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(ChargeAmount));
    
        RechargingCapsuleEffect->Modifiers.Add(EnergyModifierInfo);
        RechargingCapsuleEffect->Modifiers.Add(HealthModifierInfo);
    
        ActiveGameplayEffect = AbilitySystemComponent->ApplyGameplayEffectToSelf(RechargingCapsuleEffect, 1.0f, AbilitySystemComponent->MakeEffectContext());
    }
}

void APS_Prop_RechargingCapsule::StopInteract()
{

    if (UAbilitySystemComponent* AbilitySystemComponent = InteractingCharacter->GetComponentByClass<UAbilitySystemComponent>())
        AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGameplayEffect);
        
    Super::StopInteract();
}