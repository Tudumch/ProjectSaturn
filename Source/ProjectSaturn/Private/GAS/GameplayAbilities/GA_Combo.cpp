// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbilities/GA_Combo.h"

#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "GAS/PS_AbilitySystemStatics.h"

UGA_Combo::UGA_Combo()
{
    SetAssetTags(FGameplayTagContainer(UPS_AbilitySystemStatics::GetBasicAttackAbilityTag()));
    BlockAbilitiesWithTag.AddTag(UPS_AbilitySystemStatics::GetBasicAttackAbilityTag());
}

FGameplayTag UGA_Combo::GetComboChangedEventTag()
{
    return FGameplayTag::RequestGameplayTag("Ability.Combo");
}

FGameplayTag UGA_Combo::GetComboEndEventTag()
{
    return FGameplayTag::RequestGameplayTag("Ability.Combo.ComboEnd");
}

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) // ability in cooldown / char has not enough mana / already casting this ability ?
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    UE_LOG(LogTemp, Display, TEXT("Ability `Combo` was activated!"));

    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo)) // let client activate animmontage without waiting server (do prediction)
    {
        UAbilityTask_PlayMontageAndWait* PlayComboMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, 
        NAME_None, AbilityAnimationMontage);
        PlayComboMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility); // calls notify when montage ends play
        PlayComboMontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility); // calls notify when montage ends play
        PlayComboMontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility); // calls notify when montage ends play
        PlayComboMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility); // calls notify when montage ends play
        PlayComboMontageTask->ReadyForActivation();
        
        UAbilityTask_WaitGameplayEvent* WaitComboChangeEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent
        (this, GetComboChangedEventTag(), nullptr, false, false);
        
        WaitComboChangeEventTask->EventReceived.AddDynamic(this, &ThisClass::ComboChangedEventReceived);
        WaitComboChangeEventTask->ReadyForActivation();
    }

    SetupWaitComboInputPress();
}

void UGA_Combo::ComboChangedEventReceived(FGameplayEventData Data)
{
    if (Data.EventTag == GetComboEndEventTag())
    {
        NextComboName = NAME_None;
        return;
    }
    
    TArray<FName> TagNames;
    UGameplayTagsManager::Get().SplitGameplayTagFName(Data.EventTag, TagNames);
    NextComboName = TagNames.Last();
}

void UGA_Combo::SetupWaitComboInputPress()
{
    UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
    WaitInputPress->OnPress.AddDynamic(this, &ThisClass::HandleInputPress); 
    WaitInputPress->ReadyForActivation();
}


void UGA_Combo::HandleInputPress(float TimeWaited)
{
    SetupWaitComboInputPress(); // rebind input press event again because the old one is unbinding after activation
    TryCommitCombo();
}

void UGA_Combo::TryCommitCombo()
{
    if (NextComboName == NAME_None)
        return;

    if (UAnimInstance* AnimInstance = GetAnimationInstance())
        AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection(AbilityAnimationMontage), NextComboName, AbilityAnimationMontage);
}
