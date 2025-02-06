// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PS_CharacterBase.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/PS_EnergyComponent.h"
#include "Components/SphereComponent.h"
#include "Props/PS_Prop_Base.h"

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


void APS_CharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void APS_CharacterBase::OnInteractionRadiusOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    NearbyInteractableProp = Cast<APS_Prop_Base>(OtherActor);
    if (NearbyInteractableProp) NearbyInteractableProp->ShowTooltip(true);
}

void APS_CharacterBase::OnInteractionRadiusOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (NearbyInteractableProp) NearbyInteractableProp->ShowTooltip(false);
    NearbyInteractableProp = nullptr;
}

// ----------------------------
// Prop interaction functions
void APS_CharacterBase::Interact()
{
    if (!NearbyInteractableProp) return;

    bIsInteracting ? EndInteraction() : StartInteraction();
}

void APS_CharacterBase::StartInteraction()
{
    Interact_Starting();
    bIsInteracting = true;
    
    const FTransform PropInteractionPoint = NearbyInteractableProp->GetAnimInteractionPointTransforms();
    this->SetActorLocation(PropInteractionPoint.GetLocation());
    this->SetActorRotation(PropInteractionPoint.GetRotation());
}

void APS_CharacterBase::EndInteraction()
{
    Interact_Ending();
}

void APS_CharacterBase::Interact_Starting()
{
    float MontageDuration = PlayAnimMontage(NearbyInteractableProp->AnimationStorage.StartingInteractionAnimation);
    GetWorld()->GetTimerManager().SetTimer(MontageDurationTimer, this, &ThisClass::Interact_InProgress, MontageDuration, false);
}

void APS_CharacterBase::Interact_InProgress()
{
    GetWorldTimerManager().ClearTimer(MontageDurationTimer);
    PlayAnimMontage(NearbyInteractableProp->AnimationStorage.InteractionAnimation);
    NearbyInteractableProp->StartInteract(this);
}

void APS_CharacterBase::Interact_Ending()
{
    float MontageDuration = PlayAnimMontage(NearbyInteractableProp->AnimationStorage.EndingInteractionAnimation);
    GetWorld()->GetTimerManager().SetTimer(MontageDurationTimer, this, &ThisClass::OnFinishPlayingEndingAnimMontage, MontageDuration, false);
    
    NearbyInteractableProp->StopInteract();
}

void APS_CharacterBase::OnFinishPlayingEndingAnimMontage()
{
    bIsInteracting = false;
    GetWorldTimerManager().ClearTimer(MontageDurationTimer);
}