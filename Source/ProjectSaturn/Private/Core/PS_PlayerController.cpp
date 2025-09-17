// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PS_PlayerController.h"

#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameplayEffect.h"

#include "Core/PS_Character.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "Core/PS_GameMode.h"
#include "Components/PS_WeaponComponent.h"
#include "GAS/PS_AttributeSet.h"
#include "Systems/LoadSaveSystem/PS_LoadSaveManager.h"


void APS_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    DefineCoreVariables();

    SetShowMouseCursor(true);
}

void APS_PlayerController::SetupInputComponent()
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

void APS_PlayerController::LookAction(const FInputActionValue& Value)
{
}

void APS_PlayerController::MoveAction(const FInputActionValue& Value)
{
    if (!PS_Character) return;
    PS_Character->Move(Value);
}

void APS_PlayerController::SprintAction(const FInputActionValue& Value)
{
    if (!PS_Character) return;
    PS_Character->Run(Value);
}

void APS_PlayerController::InteractAction()
{
    if (!PS_Character) return;
    PS_Character->Interact();
}

void APS_PlayerController::AttackBaseAction(const FInputActionValue& Value)
{
    if (!WeaponComponent || PS_Character->IsInteracting()) return;

    if (Value.Get<bool>())
        WeaponComponent->AttackBaseOnPressed();
    else
        WeaponComponent->AttackBaseOnReleased();
}

void APS_PlayerController::SaveAction()
{
    if (!GameModeBase) return;

    UPS_LoadSaveManager* LoadSaveManager = GameModeBase->GetLoadSaveManager();
    if (!LoadSaveManager) return;

    LoadSaveManager->Save();
}

void APS_PlayerController::LoadAction()
{
    if (!GameModeBase) return;

    UPS_LoadSaveManager* LoadSaveManager = GameModeBase->GetLoadSaveManager();
    if (!LoadSaveManager) return;

    LoadSaveManager->Load();
}

void APS_PlayerController::DebugAddHPEnergyAction(const FInputActionValue& Value)
{
    float InputValue = Value.Get<FVector>().X;

    if (UAbilitySystemComponent* AbilitySystemComponent = PS_Character->GetAbilitySystemComponent())
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

void APS_PlayerController::DefineCoreVariables()
{
    GameModeBase = Cast<APS_GameMode>(GetWorld()->GetAuthGameMode());
    PS_Character = Cast<APS_Character>(GetPawn());
    if (!PS_Character) return;

    PS_CharacterMovementComponent = PS_Character->FindComponentByClass<UPS_CharacterMovementComponent>();
    WeaponComponent = PS_Character->FindComponentByClass<UPS_WeaponComponent>();
}