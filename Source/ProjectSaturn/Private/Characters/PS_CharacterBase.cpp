// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PS_CharacterBase.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/PS_EnergyComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PS_InteractableProp.h"

APS_CharacterBase::APS_CharacterBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UPS_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = false;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    InteractionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRadius"));
    InteractionRadius->SetSphereRadius(130);
    EnergyComponent = CreateDefaultSubobject<UPS_EnergyComponent>(TEXT("EnergyComponent"));

    SpringArm->SetupAttachment(RootComponent);
    InteractionRadius->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    InteractionRadius->OnComponentBeginOverlap.AddDynamic(this, &APS_CharacterBase::OnInteractionRadiusOverlapBegin);
    InteractionRadius->OnComponentEndOverlap.AddDynamic(this, &APS_CharacterBase::OnInteractionRadiusOverlapEnd);
}

void APS_CharacterBase::Interact()
{
    if (!NearbyInteractableProp) return;

    NearbyInteractableProp->Interact(this);
}

void APS_CharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void APS_CharacterBase::OnInteractionRadiusOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    NearbyInteractableProp = Cast<IPS_InteractableProp>(OtherActor);
    if (NearbyInteractableProp) NearbyInteractableProp->ShowTooltip(true);
}

void APS_CharacterBase::OnInteractionRadiusOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (NearbyInteractableProp) NearbyInteractableProp->ShowTooltip(false);
    NearbyInteractableProp = nullptr;
}