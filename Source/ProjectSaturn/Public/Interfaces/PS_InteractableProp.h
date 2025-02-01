// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PS_InteractableProp.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPS_InteractableProp : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTSATURN_API IPS_InteractableProp
{
	GENERATED_BODY()

public:
    UFUNCTION()
    virtual void Interact(ACharacter* Character) = 0;
    UFUNCTION()
    virtual void ShowTooltip(const bool Value) = 0;

    
};
