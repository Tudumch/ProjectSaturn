// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbilities/PS_GameplayAbility.h"

UAnimInstance* UPS_GameplayAbility::GetAnimationInstance() const
{
    if (USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponentFromActorInfo())
        return SkeletalMeshComponent->GetAnimInstance();
    
    return nullptr;
}