// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PS_EnergyComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZeroEnergy);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSATURN_API UPS_EnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPS_EnergyComponent();

    UFUNCTION(BlueprintPure)
    float GetCurrentEnergy() const { return CurrentEnergy; };
    UFUNCTION(BlueprintCallable)
    void SetCurrentEnergy(const float Value) { CurrentEnergy = Value; };
    UFUNCTION(BlueprintCallable)
    void AddEnergy(const float Value);
    UFUNCTION(BlueprintCallable)
    void ConsumeEnergy(const float Value);
    
    UPROPERTY(BlueprintAssignable)
    FOnZeroEnergy OnZeroEnergy;

protected:
    UPROPERTY()
    float CurrentEnergy;
    UPROPERTY(EditAnywhere)
    float MaxEnergy = 100.f;
    UPROPERTY(EditAnywhere)
    float EnergyConsumptionDueToBeAwake = 0.01;
    
	virtual void BeginPlay() override;

    UFUNCTION()
    virtual void ConsumeEnergyDueToBeAwake();

    

};
