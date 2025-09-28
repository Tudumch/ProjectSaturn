// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PS_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UPS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
    class UAnimInstance* GetAnimationInstance() const;
};
