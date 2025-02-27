// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PS_WeaponComponent.h"

#include "Animation/PS_AnimInstance.h"
#include "GameFramework/Character.h"

UPS_WeaponComponent::UPS_WeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPS_WeaponComponent::AttackBaseOnPressed()
{
}

void UPS_WeaponComponent::AttackBaseOnReleased()
{
}


void UPS_WeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    AnimInstance = Cast<UPS_AnimInstance>(Character->GetMesh()->GetAnimInstance());
}