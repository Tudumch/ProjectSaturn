// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PS_PlayerDeathRespawnManager.generated.h"

UCLASS()
class PROJECTSATURN_API UPS_PlayerDeathRespawnManager : public UObject
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void RespawnPlayer(APlayerController* &PlayerController);
};
