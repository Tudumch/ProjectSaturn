// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PS_Character.h"

#include "Animation/PS_AnimInstance.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/PS_WeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Props/PS_Prop_Base.h"

#include "GAS/PS_AbilitySystemComponent.h"
#include "GAS/PS_AttributeSet.h"

APS_Character::APS_Character(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UPS_CharacterMovementComponent>(
        ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = false;

    SetReplicates(true);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    InteractionRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRadius"));
    InteractionRadiusSphere->SetSphereRadius(InteractionRadius);
    WeaponComponent = CreateDefaultSubobject<UPS_WeaponComponent>(TEXT("WeaponComponent"));

    SpringArm->SetupAttachment(RootComponent);
    InteractionRadiusSphere->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    PS_AbilitySystemComponent = CreateDefaultSubobject<UPS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    PS_AttributeSet = CreateDefaultSubobject<UPS_AttributeSet>(TEXT("AttributeSet"));

    InteractionRadiusSphere->OnComponentBeginOverlap.AddDynamic(this,
        &APS_Character::OnInteractionRadiusOverlapBegin);
    InteractionRadiusSphere->OnComponentEndOverlap.AddDynamic(this, &APS_Character::OnInteractionRadiusOverlapEnd);
}


void APS_Character::BeginPlay()
{
    Super::BeginPlay();
    
    SetReplicateMovement(true);
    AnimInstance = Cast<UPS_AnimInstance>(GetMesh()->GetAnimInstance());

    PS_AbilitySystemComponent->OnZeroHealthDelegate.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);
    PS_AbilitySystemComponent->OnZeroEnergyDelegate.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);
}

void APS_Character::OnInteractionRadiusOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == this) return;

    NearbyInteractableProp = Cast<APS_Prop_Base>(OtherActor);
    if (NearbyInteractableProp)
        NearbyInteractableProp->ShowTooltip(true);
}

void APS_Character::OnInteractionRadiusOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == this) return;

    if (OtherActor == NearbyInteractableProp)
    {
        NearbyInteractableProp->ShowTooltip(false);
        NearbyInteractableProp = nullptr;
    }
}

void APS_Character::Interact()
{
    if (!NearbyInteractableProp) return;

    bIsInteracting ? EndInteraction() : StartInteraction();
}

void APS_Character::StartRespawnSequence()
{
    if (DisableSpawnAnimation) return;

    bIsInteracting = true;
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::EndInteraction);
    // delay for AnimInstance initialization
}

void APS_Character::StartDeathSequence()
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

void APS_Character::StartInteraction()
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

void APS_Character::EndInteraction()
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

void APS_Character::OnFinishPlayStartingAnimMontage()
{
    NearbyInteractableProp->StartInteract(this);
    GetWorldTimerManager().ClearTimer(MontageDurationTimer);
}

void APS_Character::OnFinishPlayEndingAnimMontage()
{
    bIsInteracting = false;
    GetWorldTimerManager().ClearTimer(MontageDurationTimer);
}

void APS_Character::OnZeroHealthEnergy(AActor* Actor)
{
    StartDeathSequence();
}

UAbilitySystemComponent* APS_Character::GetAbilitySystemComponent() const 
{
    return PS_AbilitySystemComponent;
}