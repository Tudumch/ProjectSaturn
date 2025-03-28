// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PS_EnergyComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

UPS_EnergyComponent::UPS_EnergyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    CurrentEnergy = MaxEnergy;
}

void UPS_EnergyComponent::AddEnergy(const float Value)
{
    if (CurrentEnergy <= 0) return;
    
    CurrentEnergy = FMath::Clamp(CurrentEnergy + Value, 0.f, MaxEnergy);

    if (CurrentEnergy <= 0)
        OnZeroEnergy.Broadcast(GetOwner());
}

void UPS_EnergyComponent::ConsumeEnergy(const float Value)
{
    AddEnergy(-Value);
}


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


