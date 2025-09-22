// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PS_AbilitySystemComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZeroHealthDelegate, class AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZeroEnergyDelegate, class AActor*, Actor);

UCLASS()
class PROJECTSATURN_API UPS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    UPS_AbilitySystemComponent();
    
    void GiveInitialAbilities();
    void ApplyBaseEnergyDrainEffect();

    UFUNCTION(BlueprintCallable)
    TArray<TSubclassOf<UGameplayAbility>> GetBasicAbilities() { return BasicAbilities; }

    FOnZeroHealthDelegate OnZeroHealthDelegate;
    FOnZeroEnergyDelegate OnZeroEnergyDelegate;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
    TArray<TSubclassOf<UGameplayAbility>> BasicAbilities;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
    TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;
    
    void BeginPlay() override;

    void OnAttributeValueChanged(const FOnAttributeChangeData & AttributeChangeData);
};
