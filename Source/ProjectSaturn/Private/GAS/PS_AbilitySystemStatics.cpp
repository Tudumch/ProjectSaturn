// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PS_AbilitySystemStatics.h"

FGameplayTag UPS_AbilitySystemStatics::GetBasicAttackAbilityTag()
{
    return FGameplayTag::RequestGameplayTag("Ability.BasicAttack");
}