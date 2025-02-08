// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PS_EnergyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSATURN_API UPS_EnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPS_EnergyComponent();

    UFUNCTION(BlueprintPure)
    float GetCurrentEnergy() const { return CurrentEnergy; };
    UFUNCTION(BlueprintCallable)
    void AddEnergy(const float Value);
    UFUNCTION(BlueprintCallable)
    void ConsumeEnergy(const float Value) { CurrentEnergy -= Value; };

protected:
    UPROPERTY()
    float CurrentEnergy;
    UPROPERTY(EditAnywhere)
    float MaxEnergy = 100.f;
    UPROPERTY(EditAnywhere)
    float EnergyConsumptionDueToBeAwake = 0.01;
    
	// Called when the game starts
	virtual void BeginPlay() override;

    UFUNCTION()
    virtual void ConsumeEnergyDueToBeAwake();

    

};
