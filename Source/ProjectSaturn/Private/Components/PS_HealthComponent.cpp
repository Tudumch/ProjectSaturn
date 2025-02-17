// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PS_HealthComponent.h"

UPS_HealthComponent::UPS_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    CurrentHealth = MaxHealth;
}

void UPS_HealthComponent::AddHealth(const float Value)
{
    CurrentHealth = FMath::Clamp(CurrentHealth+ Value, 0.f, MaxHealth);
}


void UPS_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

}


