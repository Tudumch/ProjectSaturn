// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PS_CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"

void UPS_CharacterMovementComponent::Move(const FInputActionValue& Value)
{
    if(!OwnerPawn) return;
    
    const FVector2D MovementVector = Value.Get<FVector2D>();
    const FRotator Rotation = GetController()->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    OwnerPawn->AddMovementInput(ForwardDirection, MovementVector.X);

    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    OwnerPawn->AddMovementInput(RightDirection, MovementVector.Y);
}

void UPS_CharacterMovementComponent::Look(const FInputActionValue& Value)
{
    if (!PlayerController) return;
    
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
    OwnerPawn = PlayerController->GetPawn();
    MaxWalkSpeedCached = MaxWalkSpeed;
}