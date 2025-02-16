// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PS_LoadSaveManager.generated.h"

UCLASS()
class PROJECTSATURN_API APS_LoadSaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APS_LoadSaveManager();

    UFUNCTION(BlueprintCallable)
    void Save();
    
    UFUNCTION(BlueprintCallable)
    void Load();

protected:
    UPROPERTY()
    class UPS_SaveGame* SaveGameObject = nullptr;
    
	virtual void BeginPlay() override;

};
