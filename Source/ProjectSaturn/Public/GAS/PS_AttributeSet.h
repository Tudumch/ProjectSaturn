// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GAS/PS_AbilitySystemComponent.h"
#include "PS_AttributeSet.generated.h"

// Macros for easy work with attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UPS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
    UPS_AttributeSet();
    
    ATTRIBUTE_ACCESSORS(UPS_AttributeSet, Health)
    ATTRIBUTE_ACCESSORS(UPS_AttributeSet, MaxHealth)
    ATTRIBUTE_ACCESSORS(UPS_AttributeSet, Energy)
    ATTRIBUTE_ACCESSORS(UPS_AttributeSet, MaxEnergy)
    ATTRIBUTE_ACCESSORS(UPS_AttributeSet, EnergyBaseConsumptionRate)
    
protected:
    UPROPERTY()
    FGameplayAttributeData Health;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayAttributeData MaxHealth;
    UPROPERTY()
    FGameplayAttributeData Energy;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayAttributeData MaxEnergy;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayAttributeData EnergyBaseConsumptionRate;

    // Called before attributes change
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
