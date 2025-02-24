// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PS_CharacterBase.h"

#include "Animation/PS_AnimInstance.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/PS_EnergyComponent.h"
#include "Components/PS_HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Props/PS_Prop_Base.h"

APS_CharacterBase::APS_CharacterBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UPS_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = false;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    InteractionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRadius"));
    InteractionRadius->SetSphereRadius(200);
    EnergyComponent = CreateDefaultSubobject<UPS_EnergyComponent>(TEXT("EnergyComponent"));
    HealthComponent = CreateDefaultSubobject<UPS_HealthComponent>(TEXT("HealthComponent"));

    SpringArm->SetupAttachment(RootComponent);
    InteractionRadius->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    InteractionRadius->OnComponentBeginOverlap.AddDynamic(this, &APS_CharacterBase::OnInteractionRadiusOverlapBegin);
    InteractionRadius->OnComponentEndOverlap.AddDynamic(this, &APS_CharacterBase::OnInteractionRadiusOverlapEnd);
}


void APS_CharacterBase::BeginPlay()
{
    Super::BeginPlay();
    AnimInstance = Cast<UPS_AnimInstance>(GetMesh()->GetAnimInstance());
}

void APS_CharacterBase::OnInteractionRadiusOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == this) return;
    
    NearbyInteractableProp = Cast<APS_Prop_Base>(OtherActor);
    if (NearbyInteractableProp)
        NearbyInteractableProp->ShowTooltip(true);
}

void APS_CharacterBase::OnInteractionRadiusOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == this) return;
    
    if (OtherActor == NearbyInteractableProp)
    {
        NearbyInteractableProp->ShowTooltip(false);
        NearbyInteractableProp = nullptr;
    }
}

// ----------------------------
// Prop interaction functions
void APS_CharacterBase::Interact()
{
    if (!NearbyInteractableProp) return;

    bIsInteracting ? EndInteraction() : StartInteraction();
}

void APS_CharacterBase::StartRespawnSequence()
{
    bIsInteracting = true;
    FTimerHandle RespawnDelayTimer; // timer is used to wait for the character AnimInstance loading
    GetWorld()->GetTimerManager().SetTimer(RespawnDelayTimer, this, &ThisClass::EndInteraction, 0.01, false);
}

void APS_CharacterBase::StartInteraction()
{
    bIsInteracting = true;
    
    if (!AnimInstance) return;
    float MontageDuration = AnimInstance->StartInteractionWithProp(NearbyInteractableProp);
    GetWorld()->GetTimerManager().SetTimer(MontageDurationTimer, this, &ThisClass::OnFinishPlayStartingAnimMontage, MontageDuration, false);
    
    const FTransform PropInteractionPoint = NearbyInteractableProp->GetAnimInteractionPointTransforms();
    this->SetActorLocation(PropInteractionPoint.GetLocation());
    this->SetActorRotation(PropInteractionPoint.GetRotation());
}

void APS_CharacterBase::EndInteraction()
{
    if (!NearbyInteractableProp)
    {
        bIsInteracting = false;
        return;
    }
    
    NearbyInteractableProp->StopInteract();
    
    if (!AnimInstance) return;
    float MontageDuration = AnimInstance->EndInteractionWithProp(NearbyInteractableProp);
    GetWorld()->GetTimerManager().SetTimer(MontageDurationTimer, this, &ThisClass::OnFinishPlayEndingAnimMontage, MontageDuration, false);
}

void APS_CharacterBase::OnFinishPlayStartingAnimMontage()
{
    NearbyInteractableProp->StartInteract(this);
    GetWorldTimerManager().ClearTimer(MontageDurationTimer);
}

void APS_CharacterBase::OnFinishPlayEndingAnimMontage()
{
    bIsInteracting = false;
    GetWorldTimerManager().ClearTimer(MontageDurationTimer);
}