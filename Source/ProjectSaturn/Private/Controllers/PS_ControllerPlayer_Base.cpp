// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/PS_ControllerPlayer_Base.h"

#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
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
    
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

    const TArray<FEnhancedActionKeyMapping>& Mappings = InputMappingContext->GetMappings();
    
    // There may be more than one keymapping assigned to one action. So, first filter duplicate action entries to prevent multiple delegate bindings
    TSet<const UInputAction*> UniqueActions;
    for (const FEnhancedActionKeyMapping& Keymapping : Mappings)
        UniqueActions.Add(Keymapping.Action);
    
    for (const UInputAction* UniqueAction : UniqueActions)
        EnhancedInputComponent->BindAction(UniqueAction, ETriggerEvent::Triggered, Cast<UObject>(this), UniqueAction->GetFName());

    // MappingContext setup
    if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubSystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        FModifyContextOptions Options;
        Options.bForceImmediately = 1;
        EnhancedInputSubSystem->AddMappingContext(InputMappingContext, 1, Options);
    }
}

void APS_ControllerPlayer_Base::LookAction(const FInputActionValue& Value)
{
    if (!PS_CharacterMovementComponent) return;

    PS_CharacterMovementComponent->Look(Value);
}

void APS_ControllerPlayer_Base::MoveAction(const FInputActionValue& Value)
{
    if (!PS_CharacterMovementComponent) return;

    PS_CharacterMovementComponent->Move(Value);
}

void APS_ControllerPlayer_Base::SprintAction(const FInputActionValue& Value)
{
    if (!PS_CharacterMovementComponent) return;

    PS_CharacterMovementComponent->Run(Value);
}

void APS_ControllerPlayer_Base::InteractAction()
{
    if (!PS_CharacterBase) return;
    PS_CharacterBase->Interact();
}

void APS_ControllerPlayer_Base::AttackBaseAction(const FInputActionValue& Value)
{
    if (!WeaponComponent || PS_CharacterBase->IsInteracting()) return;

    if (Value.Get<bool>())
        WeaponComponent->AttackBaseOnPressed();
    else
        WeaponComponent->AttackBaseOnReleased();
}

void APS_ControllerPlayer_Base::SaveAction()
{
    if (!GameModeBase) return;

    UPS_LoadSaveManager* LoadSaveManager = GameModeBase->GetLoadSaveManager();
    if (!LoadSaveManager) return;

    LoadSaveManager->Save();
}

void APS_ControllerPlayer_Base::LoadAction()
{
    if (!GameModeBase) return;

    UPS_LoadSaveManager* LoadSaveManager = GameModeBase->GetLoadSaveManager();
    if (!LoadSaveManager) return;

    LoadSaveManager->Load();
}

void APS_ControllerPlayer_Base::DebugAddHPEnergyAction(const FInputActionValue& Value)
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