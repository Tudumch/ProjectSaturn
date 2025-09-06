// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/PS_ControllerPlayer_Base.h"

#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "GameplayEffect.h"

#include "Characters/PS_CharacterBase.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "PS_GameModeBase.h"
#include "Components/PS_WeaponComponent.h"
#include "GAS/PS_AttributeSet.h"
#include "Systems/LoadSaveSystem/PS_LoadSaveManager.h"


class UGameplayEffect;

void APS_ControllerPlayer_Base::BeginPlay()
{
    Super::BeginPlay();

    DefineCoreVariables();

    SetShowMouseCursor(true);
}

void APS_ControllerPlayer_Base::SetupInputComponent()
{
    Super::SetupInputComponent();

    // MappingContext setup
    if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubSystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        EnhancedInputSubSystem->AddMappingContext(InputMappingContext, 2);
    }

    // InputActions setup
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
    if (!EnhancedInputComponent) return;

    EnhancedInputComponent->BindAction(PauseIA, ETriggerEvent::Completed, this, &ThisClass::Pause);
    EnhancedInputComponent->BindAction(MoveIA, ETriggerEvent::Triggered, this, &ThisClass::Move);
    EnhancedInputComponent->BindAction(LookIA, ETriggerEvent::Triggered, this, &ThisClass::Look);
    EnhancedInputComponent->BindAction(RunIA, ETriggerEvent::Started, this, &ThisClass::ToggleRun);
    EnhancedInputComponent->BindAction(InteractIA, ETriggerEvent::Started, this, &ThisClass::Interact);
    EnhancedInputComponent->BindAction(AttackBaseIA, ETriggerEvent::Started, this, &ThisClass::AttackBasePressed);
    EnhancedInputComponent->BindAction(AttackBaseIA, ETriggerEvent::Completed, this, &ThisClass::AttackBaseReleased);
    EnhancedInputComponent->BindAction(SaveIA, ETriggerEvent::Started, this, &ThisClass::Save);
    EnhancedInputComponent->BindAction(LoadIA, ETriggerEvent::Started, this, &ThisClass::Load);

    // Debug IAs
    EnhancedInputComponent->BindAction(DebugAddHPEnergyIA, ETriggerEvent::Triggered, this,
        &ThisClass::DebugAddHPEnergy);
}

void APS_ControllerPlayer_Base::Look(const FInputActionValue& Value)
{
    if (!PS_CharacterMovementComponent) return;

    PS_CharacterMovementComponent->Look(Value);
}

void APS_ControllerPlayer_Base::Move(const FInputActionValue& Value)
{
    if (!PS_CharacterMovementComponent) return;

    PS_CharacterMovementComponent->Move(Value);
}

void APS_ControllerPlayer_Base::ToggleRun()
{
    if (!PS_CharacterMovementComponent) return;

    PS_CharacterMovementComponent->Run();
}

void APS_ControllerPlayer_Base::Interact()
{
    if (!PS_CharacterBase) return;
    PS_CharacterBase->Interact();
}

void APS_ControllerPlayer_Base::AttackBasePressed()
{
    if (!WeaponComponent || PS_CharacterBase->IsInteracting()) return;
    WeaponComponent->AttackBaseOnPressed();
}

void APS_ControllerPlayer_Base::AttackBaseReleased()
{
    if (!WeaponComponent) return;
    WeaponComponent->AttackBaseOnReleased();
}

void APS_ControllerPlayer_Base::Save()
{
    if (!GameModeBase) return;

    UPS_LoadSaveManager* LoadSaveManager = GameModeBase->GetLoadSaveManager();
    if (!LoadSaveManager) return;

    LoadSaveManager->Save();
}

void APS_ControllerPlayer_Base::Load()
{
    if (!GameModeBase) return;

    UPS_LoadSaveManager* LoadSaveManager = GameModeBase->GetLoadSaveManager();
    if (!LoadSaveManager) return;

    LoadSaveManager->Load();
}

void APS_ControllerPlayer_Base::DebugAddHPEnergy(const FInputActionValue& Value)
{
    float InputValue = Value.Get<FVector>().X;

    if (UAbilitySystemComponent* AbilitySystemComponent = PS_CharacterBase->GetAbilitySystemComponent())
    {
        // HP
        UGameplayEffect* HP_Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("MovementEnergyDrainEffect")));
        HP_Effect->DurationPolicy = EGameplayEffectDurationType::Instant;

        FGameplayModifierInfo HP_ModifierInfo;
        HP_ModifierInfo.Attribute = UPS_AttributeSet::GetHealthAttribute();
        HP_ModifierInfo.ModifierOp = EGameplayModOp::Additive;
        HP_ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(InputValue));

        HP_Effect->Modifiers.Add(HP_ModifierInfo);

        AbilitySystemComponent->ApplyGameplayEffectToSelf(HP_Effect, 1.0f, AbilitySystemComponent->MakeEffectContext());

        // Energy
        UGameplayEffect* Energy_Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("MovementEnergyDrainEffect")));
        Energy_Effect->DurationPolicy = EGameplayEffectDurationType::Instant;

        FGameplayModifierInfo Energy_ModifierInfo;
        Energy_ModifierInfo.Attribute = UPS_AttributeSet::GetEnergyAttribute();
        Energy_ModifierInfo.ModifierOp = EGameplayModOp::Additive;
        Energy_ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(InputValue));

        Energy_Effect->Modifiers.Add(Energy_ModifierInfo);

        AbilitySystemComponent->ApplyGameplayEffectToSelf(Energy_Effect, 1.0f, AbilitySystemComponent->MakeEffectContext());
    }
}

void APS_ControllerPlayer_Base::DefineCoreVariables()
{
    GameModeBase = Cast<APS_GameModeBase>(GetWorld()->GetAuthGameMode());
    PS_CharacterBase = Cast<APS_CharacterBase>(GetPawn());
    if (!PS_CharacterBase) return;

    PS_CharacterMovementComponent = PS_CharacterBase->FindComponentByClass<UPS_CharacterMovementComponent>();
    WeaponComponent = PS_CharacterBase->FindComponentByClass<UPS_WeaponComponent>();
}