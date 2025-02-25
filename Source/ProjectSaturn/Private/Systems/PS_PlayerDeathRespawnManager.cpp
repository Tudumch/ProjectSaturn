// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/PS_PlayerDeathRespawnManager.h"

#include "Characters/PS_CharacterBase.h"
#include "Components/PS_EnergyComponent.h"
#include "Components/PS_HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Props/PS_Prop_RechargingCapsule.h"

void UPS_PlayerDeathRespawnManager::SpawnPlayer(APlayerController* PlayerController)
{
    if (!PlayerController) return;

    APS_CharacterBase* Character = Cast<APS_CharacterBase>(PlayerController->GetCharacter());
    if (Character == nullptr) return;

    if (UPS_HealthComponent* HealthComponent = Character->GetComponentByClass<UPS_HealthComponent>())
        HealthComponent->OnZeroHealth.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);
    if (UPS_EnergyComponent* EnergyComponent = Character->GetComponentByClass<UPS_EnergyComponent>())
        EnergyComponent->OnZeroEnergy.AddDynamic(this, &ThisClass::OnZeroHealthEnergy);

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

    if (UPS_HealthComponent* HealthComponent = Character->GetComponentByClass<UPS_HealthComponent>())
        HealthComponent->ResetHealth();
    if (UPS_EnergyComponent* EnergyComponent = Character->GetComponentByClass<UPS_EnergyComponent>())
        EnergyComponent->ResetEnergy();

    Character->GetRootComponent()->SetWorldTransform(RechargingCapsule->GetAnimInteractionPointTransforms(), false,
        nullptr, ETeleportType::TeleportPhysics);
    
    Character->StartRespawnSequence();

    PlayerController->PlayerCameraManager->StartCameraFade(1, 0, 5, FColor::Black, true, false);
}

void UPS_PlayerDeathRespawnManager::OnZeroHealthEnergy(AActor* Actor)
{
    if (!Actor) return;

    APS_CharacterBase* Character = Cast<APS_CharacterBase>(Actor);
    if (!Character) return;

    float DeathSequenceLength = Character->StartDeathSequence();

    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    if (!PlayerController) return;

    PlayerController->PlayerCameraManager->StartCameraFade(0, 1, DeathSequenceLength / 2, FLinearColor::Black, true,
        true);

    FTimerHandle DeathSequenceTimer;
    GetWorld()->GetTimerManager().SetTimer(DeathSequenceTimer,
        [this, PlayerController]() { ThisClass::RespawnPlayer(PlayerController); }, DeathSequenceLength,
        false); 
}
