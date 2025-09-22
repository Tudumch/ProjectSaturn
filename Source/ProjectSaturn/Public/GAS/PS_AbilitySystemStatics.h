// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PS_AbilitySystemStatics.generated.h"

/**
 * 
 */
UCLASS()
class UPS_AbilitySystemStatics : public UObject
{
    GENERATED_BODY()
public:
    static FGameplayTag GetBasicAttackAbilityTag();
};
