// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackerMelee.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackerMelee : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTSATURN_API IAttackerMelee
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION()
    virtual void DoMeleeAttack() = 0;
};
