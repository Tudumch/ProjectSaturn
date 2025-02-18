// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PS_LoadSaveManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UPS_LoadSaveManager : public UObject
{
	GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable)
    void Save();
    
    UFUNCTION(BlueprintCallable)
    void Load();
    
    UFUNCTION(BlueprintCallable)
    void InitializeSaveProcess();

protected:
    UPROPERTY()
    class UPS_SaveGame* SaveGameObject = nullptr;
};
