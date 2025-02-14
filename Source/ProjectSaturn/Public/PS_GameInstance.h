// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PS_GameInstance.generated.h"

class APS_LoadSaveManager;
/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UPS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    APS_LoadSaveManager* GetLoadSaveManager() const { return LoadSaveManager; };
    
protected:
    UPROPERTY()
    APS_LoadSaveManager* LoadSaveManager;
    
    virtual void Init() override;
};
