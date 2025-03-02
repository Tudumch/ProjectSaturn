// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PS_WeaponComponent.h"

#include "Animation/PS_AnimInstance.h"
#include "GameFramework/Character.h"
#include "Weapons/PS_WeaponBase.h"

UPS_WeaponComponent::UPS_WeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPS_WeaponComponent::AttackBaseOnPressed()
{
    if (!ActiveWeapon) return;

    bIsAttacking = true;
    ActiveWeapon->StartFire();
    
    if (AnimInstance)
    AnimInstance->Montage_Play(ActiveWeapon->GetFireMontage());
}

void UPS_WeaponComponent::AttackBaseOnReleased()
{
}


void UPS_WeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    AnimInstance = Cast<UPS_AnimInstance>(Character->GetMesh()->GetAnimInstance());

    ActiveWeapon = GetWorld()->SpawnActor<APS_WeaponBase>(InitialWeaponClass);
    ActiveWeapon->SetOwner(GetOwner());
    ActiveWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
        WeaponSocketRightName);

}