// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/PS_PlayerDeathRespawnManager.h"

#include "Kismet/GameplayStatics.h"
#include "Props/PS_Prop_RechargingCapsule.h"

void UPS_PlayerDeathRespawnManager::RespawnPlayer(APlayerController* &PlayerController)
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(this, APS_Prop_RechargingCapsule::StaticClass(), FoundActors);
    if (FoundActors.Num() == 0) return;
    
    
    // FTransform RespawnTransform = FoundActors[0]->GetTransform();
    // ACharacter* Character = GetWorld()->SpawnActor<APS_CharacterBase>(APS_CharacterBase::StaticClass(), RespawnTransform);
    // PlayerController->Possess(Character);
}