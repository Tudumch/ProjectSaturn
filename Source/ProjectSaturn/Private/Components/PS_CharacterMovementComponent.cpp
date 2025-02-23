// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PS_CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"

#include "Characters/PS_CharacterBase.h"

#include "Components/PS_EnergyComponent.h"

void UPS_CharacterMovementComponent::Move(const FInputActionValue& Value)
{
    if(!OwnerCharacter || !EnergyComponent) return;
    if (OwnerCharacter->IsInteracting()) return;

    float EnergyNeeded;
    IsRunning ? EnergyNeeded = EnergyForRunning : EnergyNeeded = EnergyForWalking;
    if (EnergyComponent->GetCurrentEnergy() < EnergyNeeded) return;
    
    const FVector2D MovementVector = Value.Get<FVector2D>();
    const FRotator Rotation = GetController()->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    OwnerCharacter->AddMovementInput(ForwardDirection, MovementVector.X);

    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    OwnerCharacter->AddMovementInput(RightDirection, MovementVector.Y);

    EnergyComponent->ConsumeEnergy(EnergyNeeded);
}

void UPS_CharacterMovementComponent::Look(const FInputActionValue& Value)
{
    if (!PlayerController)
    {
        PlayerController = Cast<APlayerController>(GetController());
        if (!PlayerController) return;
    }
    
    const FVector2D LookAxisValue = Value.Get<FVector2D>();
    
    PlayerController->AddYawInput(LookAxisValue.X);
    PlayerController->AddPitchInput(LookAxisValue.Y);
}

void UPS_CharacterMovementComponent::Run()
{
    IsRunning = !IsRunning;
    IsRunning ? MaxWalkSpeed = MaxRunSpeed : MaxWalkSpeed = MaxWalkSpeedCached;
}

void UPS_CharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<APlayerController>(GetController());
    OwnerCharacter = Cast<APS_CharacterBase>(GetOwner());
    EnergyComponent = OwnerCharacter->FindComponentByClass<UPS_EnergyComponent>();
    MaxWalkSpeedCached = MaxWalkSpeed;
}