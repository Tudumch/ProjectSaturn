// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/PS_PlayerDeathRespawnManager.h"

#include "Characters/PS_CharacterBase.h"
#include "Components/PS_EnergyComponent.h"
#include "Components/PS_HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Props/PS_Prop_RechargingCapsule.h"

void UPS_PlayerDeathRespawnManager::RespawnPlayer(APlayerController* &PlayerController)
{
    if (!PlayerController) return;
    
    TArray<AActor*> RechargingCapsules;
    UGameplayStatics::GetAllActorsOfClass(this, APS_Prop_RechargingCapsule::StaticClass(), RechargingCapsules);
    if (RechargingCapsules.Num() == 0) return;
    const APS_Prop_RechargingCapsule* RechargingCapsule = Cast<APS_Prop_RechargingCapsule>(RechargingCapsules[0]);
    
    APS_CharacterBase* Character = Cast<APS_CharacterBase>(PlayerController->GetCharacter());
    if (Character == nullptr) return;
    
    Character->GetRootComponent()->SetWorldTransform(RechargingCapsule->GetAnimInteractionPointTransforms(), false, nullptr, ETeleportType::TeleportPhysics);
    Character->GetComponentByClass<UPS_HealthComponent>()->ResetHealth();
    Character->GetComponentByClass<UPS_EnergyComponent>()->ResetEnergy();
    Character->StartRespawnSequence();

}