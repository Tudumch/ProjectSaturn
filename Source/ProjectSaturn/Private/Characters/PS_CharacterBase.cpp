// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PS_CharacterBase.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/PS_EnergyComponent.h"

APS_CharacterBase::APS_CharacterBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UPS_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = false;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    EnergyComponent = CreateDefaultSubobject<UPS_EnergyComponent>(TEXT("EnergyComponent"));

    SpringArm->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void APS_CharacterBase::BeginPlay()
{
    Super::BeginPlay();
}