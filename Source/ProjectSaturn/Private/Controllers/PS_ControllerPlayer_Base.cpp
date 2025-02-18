// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/PS_ControllerPlayer_Base.h"

#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"

#include "Characters/PS_CharacterBase.h"
#include "Components/PS_CharacterMovementComponent.h"
#include "PS_GameInstance.h"
#include "Systems/LoadSaveSystem/PS_LoadSaveManager.h"


void APS_ControllerPlayer_Base::BeginPlay()
{
    Super::BeginPlay();

    DefineCoreVariables();
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
    EnhancedInputComponent->BindAction(SaveIA, ETriggerEvent::Started, this, &ThisClass::Save);
    EnhancedInputComponent->BindAction(LoadIA, ETriggerEvent::Started, this, &ThisClass::Load);
}

void APS_ControllerPlayer_Base::Look(const FInputActionValue& Value)
{
    if (!PS_CharacterMovementComponent) return;

    PS_CharacterMovementComponent->Look(Value);
}

void APS_ControllerPlayer_Base::Move(const FInputActionValue& Value)
{
    if (!PS_CharacterMovementComponent ) return;

    PS_CharacterMovementComponent->Move(Value);
}

void APS_ControllerPlayer_Base::ToggleRun()
{
    if (!PS_CharacterMovementComponent ) return;

    PS_CharacterMovementComponent->Run();
}

void APS_ControllerPlayer_Base::Interact()
{
    if (!PS_CharacterBase) return;
    PS_CharacterBase->Interact();
}

void APS_ControllerPlayer_Base::Save()
{
    const UPS_GameInstance* GameInstance = Cast<UPS_GameInstance>(GetGameInstance());
    if (!GameInstance) return;

    // APS_LoadSaveManager* LoadSaveManager = GameInstance->GetLoadSaveManager();
    UPS_LoadSaveManager* LoadSaveManager = GameInstance->GetLoadSaveManager();
    if (!LoadSaveManager) return;

    LoadSaveManager->Save();
}

void APS_ControllerPlayer_Base::Load()
{
    const UPS_GameInstance* GameInstance = Cast<UPS_GameInstance>(GetGameInstance());
    if (!GameInstance) return;
    
    // APS_LoadSaveManager* LoadSaveManager = GameInstance->GetLoadSaveManager();
    UPS_LoadSaveManager* LoadSaveManager = GameInstance->GetLoadSaveManager();
    if (!LoadSaveManager) return;

    LoadSaveManager->Load();
}

void APS_ControllerPlayer_Base::DefineCoreVariables()
{
    PS_CharacterBase = Cast<APS_CharacterBase>(GetPawn());
    if (!PS_CharacterBase) return;
    
    PS_CharacterMovementComponent = PS_CharacterBase->FindComponentByClass<UPS_CharacterMovementComponent>();
}