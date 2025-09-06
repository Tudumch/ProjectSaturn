// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PS_CharacterBase.h"

#include "Animation/PS_AnimInstance.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/PS_EnergyComponent.h"
#include "Components/PS_HealthComponent.h"
#include "Components/PS_WeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Props/PS_Prop_Base.h"

#include "GAS/PS_AbilitySystemComponent.h"
#include "GAS/PS_AttributeSet.h"

APS_CharacterBase::APS_CharacterBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UPS_CharacterMovementComponent>(
        ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = false;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    InteractionRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRadius"));
    InteractionRadiusSphere->SetSphereRadius(InteractionRadius);
    EnergyComponent = CreateDefaultSubobject<UPS_EnergyComponent>(TEXT("EnergyComponent"));
    HealthComponent = CreateDefaultSubobject<UPS_HealthComponent>(TEXT("HealthComponent"));
    WeaponComponent = CreateDefaultSubobject<UPS_WeaponComponent>(TEXT("WeaponComponent"));

    SpringArm->SetupAttachment(RootComponent);
    InteractionRadiusSphere->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    PS_AbilitySystemComponent = CreateDefaultSubobject<UPS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    PS_AttributeSet = CreateDefaultSubobject<UPS_AttributeSet>(TEXT("AttributeSet"));

    InteractionRadiusSphere->OnComponentBeginOverlap.AddDynamic(this,
        &APS_CharacterBase::OnInteractionRadiusOverlapBegin);
    InteractionRadiusSphere->OnComponentEndOverlap.AddDynamic(this, &APS_CharacterBase::OnInteractionRadiusOverlapEnd);
}


void APS_CharacterBase::BeginPlay()
{
    Super::BeginPlay();
    AnimInstance = Cast<UPS_AnimInstance>(GetMesh()->GetAnimInstance());

    PS_AbilitySystemComponent->OnZeroHealthDelegate.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);
    PS_AbilitySystemComponent->OnZeroEnergyDelegate.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);
    PS_AbilitySystemComponent->ApplyBaseEnergyDrainEffect();
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

void APS_CharacterBase::Interact()
{
    if (!NearbyInteractableProp) return;

    bIsInteracting ? EndInteraction() : StartInteraction();
}

void APS_CharacterBase::StartRespawnSequence()
{
    if (DisableSpawnAnimation) return;

    bIsInteracting = true;
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::EndInteraction);
    // delay for AnimInstance initialization
}

void APS_CharacterBase::StartDeathSequence()
{
    bIsInteracting = true;

    if (!DeathAnimation)
        UE_LOG(LogTemp, Error, TEXT("%s: death animation doesn't set!"), *GetName());

    const float SequenceLength = AnimInstance->Montage_Play(DeathAnimation);

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    // TODO: transfer this to CharacterPlayer class, when it will be created
    {
        PlayerController->PlayerCameraManager->StartCameraFade(0, 1, SequenceLength / 2, FLinearColor::Black, true,
            true);
    }
}

void APS_CharacterBase::StartInteraction()
{
    bIsInteracting = true;

    if (!AnimInstance) return;
    float MontageDuration = AnimInstance->StartInteractionWithProp(NearbyInteractableProp);

    GetWorld()->GetTimerManager().SetTimer(MontageDurationTimer, this, &ThisClass::OnFinishPlayStartingAnimMontage,
        MontageDuration, false);

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

    GetWorld()->GetTimerManager().SetTimer(MontageDurationTimer, this, &ThisClass::OnFinishPlayEndingAnimMontage,
        MontageDuration, false);
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

void APS_CharacterBase::OnZeroHealthEnergy(AActor* Actor)
{
    StartDeathSequence();
}

UAbilitySystemComponent* APS_CharacterBase::GetAbilitySystemComponent() const 
{
    return PS_AbilitySystemComponent;
}