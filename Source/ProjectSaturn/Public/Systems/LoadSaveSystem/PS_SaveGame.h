// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PS_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UPS_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
    FTransform PlayerTransform;
};
