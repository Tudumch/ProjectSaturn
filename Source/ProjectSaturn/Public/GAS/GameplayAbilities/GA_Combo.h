// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/PS_GameplayAbility.h"
#include "GA_Combo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UGA_Combo : public UPS_GameplayAbility
{
	GENERATED_BODY()

public:
    UGA_Combo();
    static FGameplayTag GetComboChangedEventTag();
    static FGameplayTag GetComboEndEventTag();
    
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    
    UFUNCTION()
    void ComboChangedEventReceived(FGameplayEventData Data);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
    UAnimMontage* AbilityAnimationMontage;

    FName NextComboName;

    void SetupWaitComboInputPress();
    void TryCommitCombo();
    
    UFUNCTION()
    void HandleInputPress(float TimeWaited);
};
