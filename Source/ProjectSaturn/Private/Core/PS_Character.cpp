// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PS_Character.h"

#include "Animation/PS_AnimInstance.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

#include "Components/PS_WeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Props/PS_Prop_Base.h"

#include "GAS/PS_AbilitySystemComponent.h"
#include "GAS/PS_AttributeSet.h"

APS_Character::APS_Character(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UPS_CharacterMovementComponent>(
        ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SetReplicates(true);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    InteractionRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRadius"));
    InteractionRadiusSphere->SetSphereRadius(InteractionRadius);
    WeaponComponent = CreateDefaultSubobject<UPS_WeaponComponent>(TEXT("WeaponComponent"));

    SpringArm->SetupAttachment(RootComponent);
    InteractionRadiusSphere->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    AbilitySystemComponent = CreateDefaultSubobject<UPS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AttributeSet = CreateDefaultSubobject<UPS_AttributeSet>(TEXT("AttributeSet"));

    InteractionRadiusSphere->OnComponentBeginOverlap.AddDynamic(this,
        &APS_Character::OnInteractionRadiusOverlapBegin);
    InteractionRadiusSphere->OnComponentEndOverlap.AddDynamic(this, &APS_Character::OnInteractionRadiusOverlapEnd);
}

void APS_Character::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(APS_Character, IsRunning);
    DOREPLIFETIME(APS_Character, ReplicatedRotation);
}


void APS_Character::BeginPlay()
{
    Super::BeginPlay();
    
    AnimInstance = Cast<UPS_AnimInstance>(GetMesh()->GetAnimInstance());
    MaxWalkSpeedCached = GetCharacterMovement()->MaxWalkSpeed;

    AbilitySystemComponent->OnZeroHealthDelegate.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);
    AbilitySystemComponent->OnZeroEnergyDelegate.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);

    if (IsLocallyControlled())
        GetWorldTimerManager().SetTimer(ReplicationTimer, this, &ThisClass::DoReplication, ReplicationInterval, true);
}

void APS_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsLocallyControlled() && bShouldFollowCursor)
        RotateToMouseCursor();
}

void APS_Character::DoReplication()
{
    if (!IsLocallyControlled()) return;

    Server_UpdateRotation(GetActorRotation());
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

void APS_Character::ApplyEnergyDrainEffect(const float EnergyDrainAmount) const
{
    if (!AbilitySystemComponent) return;
    
    UGameplayEffect* EnergyDrainEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("MovementEnergyDrainEffect")));
    EnergyDrainEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UPS_AttributeSet::GetEnergyAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-EnergyDrainAmount));

    EnergyDrainEffect->Modifiers.Add(ModifierInfo);

    AbilitySystemComponent->ApplyGameplayEffectToSelf(EnergyDrainEffect, 1.0f, AbilitySystemComponent->MakeEffectContext());
}

void APS_Character::RotateToMouseCursor()
{
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController) return;

    // Get mouse hit result
    FHitResult HitResult;
    bool bHitSuccessful = PlayerController->GetHitResultUnderCursor(
        ECollisionChannel::ECC_Visibility,
        true,
        HitResult
    );

    if (bHitSuccessful)
    {
        FVector MouseWorldLocation = HitResult.Location;
        
        FVector Direction = MouseWorldLocation - GetActorLocation();
        Direction.Z = 0; 
        Direction.Normalize();

        FRotator TargetRotation = Direction.Rotation();

        // Rotation interpolation
        FRotator CurrentRotation = GetActorRotation();
        FRotator NewRotation = FMath::RInterpTo(
            CurrentRotation,
            TargetRotation,
            GetWorld()->GetDeltaSeconds(),
            100
        );

        SetActorRotation(FRotator(0, NewRotation.Yaw, 0));
    }
}

void APS_Character::OnRep_ReplicatedRotation()
{
    SetActorRotation(ReplicatedRotation);
}

void APS_Character::Server_UpdateRotation_Implementation(const FRotator NewRotation)
{
    ReplicatedRotation = NewRotation;
    SetActorRotation(ReplicatedRotation);
}

UAbilitySystemComponent* APS_Character::GetAbilitySystemComponent() const 
{
    return AbilitySystemComponent;
}

void APS_Character::Move(const FInputActionValue& Value)
{
    if(IsInteracting()) return;

    float EnergyNeeded;
    IsRunning ? EnergyNeeded = EnergyForRunning : EnergyNeeded = EnergyForWalking;

    if (AbilitySystemComponent)
    {
        bool bIsAttrValid = false;
        float CurrentEnergy = AbilitySystemComponent->GetGameplayAttributeValue(UPS_AttributeSet::GetEnergyAttribute(), bIsAttrValid);
        
        if (!bIsAttrValid || CurrentEnergy < EnergyNeeded)
            return;
    }

    const FVector2D MovementVector = Value.Get<FVector2D>();
    const FRotator Rotation = GetController()->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, MovementVector.X);

    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementVector.Y);

    ApplyEnergyDrainEffect(EnergyNeeded);
}

void APS_Character::Run(const FInputActionValue& Value)
{
    IsRunning = Value.Get<bool>();

    if (!GetOwner()->HasAuthority())
        Server_Run(IsRunning);
    else
        Multicast_Run(IsRunning);
}

void APS_Character::Server_Run_Implementation(const bool bWantsToRun)
{
    Multicast_Run(bWantsToRun);
}

void APS_Character::Multicast_Run_Implementation(const bool bWantsToRun)
{
    IsRunning = bWantsToRun;
    GetCharacterMovement()->MaxWalkSpeed = bWantsToRun ? MaxRunSpeed : MaxWalkSpeedCached;
}