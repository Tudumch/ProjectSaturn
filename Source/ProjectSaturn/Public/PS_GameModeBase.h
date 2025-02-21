// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PS_GameModeBase.generated.h"

class UPS_LoadSaveManager;
class UPS_PlayerDeathRespawnManager;

UCLASS()
class PROJECTSATURN_API APS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    UPS_LoadSaveManager* GetLoadSaveManager() const { return LoadSaveManager; };
    
protected:
    UPROPERTY()
    UPS_LoadSaveManager* LoadSaveManager;
    UPROPERTY()
    UPS_PlayerDeathRespawnManager* DeathRespawnManager;
    
    virtual void BeginPlay() override;
};
