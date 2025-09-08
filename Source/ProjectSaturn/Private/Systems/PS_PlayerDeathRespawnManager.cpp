// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/PS_PlayerDeathRespawnManager.h"

#include "Characters/PS_CharacterBase.h"
#include "GAS/PS_AbilitySystemComponent.h"
#include "GAS/PS_AttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Props/PS_Prop_RechargingCapsule.h"

void UPS_PlayerDeathRespawnManager::SpawnPlayer(APlayerController* PlayerController)
{
    if (!PlayerController) return;

    APS_CharacterBase* Character = Cast<APS_CharacterBase>(PlayerController->GetCharacter());
    if (Character == nullptr) return;

    if (UPS_AbilitySystemComponent* AbilitySystemComponent = Cast<UPS_AbilitySystemComponent>(Character->GetAbilitySystemComponent()))
    {
        AbilitySystemComponent->OnZeroHealthDelegate.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);
        AbilitySystemComponent->OnZeroEnergyDelegate.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);
    }

    RespawnPlayer(PlayerController);
}

void UPS_PlayerDeathRespawnManager::RespawnPlayer(APlayerController* PlayerController)
{
    if (!PlayerController) return;

    TArray<AActor*> RechargingCapsules;
    UGameplayStatics::GetAllActorsOfClass(this, APS_Prop_RechargingCapsule::StaticClass(), RechargingCapsules);
    if (RechargingCapsules.Num() == 0) return;
    const APS_Prop_RechargingCapsule* RechargingCapsule = Cast<APS_Prop_RechargingCapsule>(RechargingCapsules[0]);

    APS_CharacterBase* Character = Cast<APS_CharacterBase>(PlayerController->GetCharacter());
    if (Character == nullptr) return;

    if (UPS_AbilitySystemComponent* AbilitySystemComponent = Cast<UPS_AbilitySystemComponent>(Character->GetAbilitySystemComponent()))
    {
        const UPS_AttributeSet* DefaultAttributeSet = GetDefault<UPS_AttributeSet>();
        AbilitySystemComponent->SetNumericAttributeBase(UPS_AttributeSet::GetHealthAttribute(), DefaultAttributeSet->GetHealth());
        AbilitySystemComponent->SetNumericAttributeBase(UPS_AttributeSet::GetEnergyAttribute(), DefaultAttributeSet->GetEnergy());
    }
    
    Character->GetRootComponent()->SetWorldTransform(RechargingCapsule->GetAnimInteractionPointTransforms(), false,
        nullptr, ETeleportType::TeleportPhysics);
    
    Character->StartRespawnSequence();

    PlayerController->PlayerCameraManager->StartCameraFade(1, 0, 5, FColor::Black, true, false);
}

void UPS_PlayerDeathRespawnManager::OnZeroHealthEnergy(AActor* Actor)
{
    if (!Actor) return;

    const APS_CharacterBase* Character = Cast<APS_CharacterBase>(Actor);
    if (!Character) return;

    const float DeathSequenceLength = Character->GetDeathAnimMontage()->GetPlayLength();

    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    if (!PlayerController) return;

    FTimerHandle DeathSequenceTimer;
    GetWorld()->GetTimerManager().SetTimer(DeathSequenceTimer,
        [this, PlayerController]() { ThisClass::RespawnPlayer(PlayerController); }, DeathSequenceLength,
        false); 
}
