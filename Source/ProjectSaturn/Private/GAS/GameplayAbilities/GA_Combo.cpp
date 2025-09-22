// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbilities/GA_Combo.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS/PS_AbilitySystemStatics.h"

UGA_Combo::UGA_Combo()
{
    SetAssetTags(FGameplayTagContainer(UPS_AbilitySystemStatics::GetBasicAttackAbilityTag()));
    BlockAbilitiesWithTag.AddTag(UPS_AbilitySystemStatics::GetBasicAttackAbilityTag());
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
    }
}