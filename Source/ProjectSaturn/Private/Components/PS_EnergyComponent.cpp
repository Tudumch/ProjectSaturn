// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PS_EnergyComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UPS_EnergyComponent::UPS_EnergyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPS_EnergyComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentEnergy = MaxEnergy;

    FTimerHandle EnergyAwakeConsumptionTimerHandle;
    FTimerDynamicDelegate EnergyAwakeConsumptionDelegate;
    EnergyAwakeConsumptionDelegate.BindDynamic(this, &UPS_EnergyComponent::ConsumeEnergyDueToBeAwake);
    GetWorld()->GetTimerManager().SetTimer(EnergyAwakeConsumptionTimerHandle, EnergyAwakeConsumptionDelegate, 1.f, true);
}

void UPS_EnergyComponent::ConsumeEnergyDueToBeAwake()
{
    ConsumeEnergy(EnergyConsumptionDueToBeAwake);
}


