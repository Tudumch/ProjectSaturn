// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PS_CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "GameplayEffect.h"

#include "Characters/PS_CharacterBase.h"
#include "GAS/PS_AttributeSet.h"

void UPS_CharacterMovementComponent::Move(const FInputActionValue& Value)
{
    if(!OwnerCharacter || OwnerCharacter->IsInteracting()) return;

    float EnergyNeeded;
    IsRunning ? EnergyNeeded = EnergyForRunning : EnergyNeeded = EnergyForWalking;

    if (AbilitySystemComponent)
    {
        bool bIsAttrValid = false;
        float CurrentEnergy = AbilitySystemComponent->GetGameplayAttributeValue(UPS_AttributeSet::GetEnergyAttribute(), bIsAttrValid);
        
        if (!bIsAttrValid || CurrentEnergy < EnergyNeeded)
            return;
    }
    
    const FVector2D MovementVector = Value.Get<FVector2D>();
    const FRotator Rotation = GetController()->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    OwnerCharacter->AddMovementInput(ForwardDirection, MovementVector.X);

    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    OwnerCharacter->AddMovementInput(RightDirection, MovementVector.Y);

    ApplyEnergyDrainEffect(EnergyNeeded);
}

void UPS_CharacterMovementComponent::Look(const FInputActionValue& Value)
{
    // Disabled because of new project conception.
    return;

    // TODO: delete after new project conception be approved
    // if (!PlayerController)
    // {
    //     PlayerController = Cast<APlayerController>(GetController());
    //     if (!PlayerController) return;
    // }
    //
    // const FVector2D LookAxisValue = Value.Get<FVector2D>();
    //
    // PlayerController->AddYawInput(LookAxisValue.X);
    // PlayerController->AddPitchInput(LookAxisValue.Y);
}

void UPS_CharacterMovementComponent::Run(const FInputActionValue& Value)
{
    IsRunning = Value.Get<bool>();
    IsRunning ? MaxWalkSpeed = MaxRunSpeed : MaxWalkSpeed = MaxWalkSpeedCached;
}

void UPS_CharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<APlayerController>(GetController());
    OwnerCharacter = Cast<APS_CharacterBase>(GetOwner());
    if (OwnerCharacter)
        AbilitySystemComponent = OwnerCharacter->GetAbilitySystemComponent();
    
    MaxWalkSpeedCached = MaxWalkSpeed;
}

void UPS_CharacterMovementComponent::ApplyEnergyDrainEffect(const float EnergyDrainAmount) const
{
    if (!OwnerCharacter || !AbilitySystemComponent) return;
    
    UGameplayEffect* EnergyDrainEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("MovementEnergyDrainEffect")));
    EnergyDrainEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UPS_AttributeSet::GetEnergyAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-EnergyDrainAmount));

    EnergyDrainEffect->Modifiers.Add(ModifierInfo);

    AbilitySystemComponent->ApplyGameplayEffectToSelf(EnergyDrainEffect, 1.0f, AbilitySystemComponent->MakeEffectContext());
}