// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_GameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Systems/PS_PlayerDeathRespawnManager.h"
#include "Systems/LoadSaveSystem/PS_LoadSaveManager.h"

void APS_GameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // TODO: Save system temporary disabled until respawn-system will be completed
    // LoadSaveManager = NewObject<UPS_LoadSaveManager>(this, UPS_LoadSaveManager::StaticClass());
    // LoadSaveManager->InitializeSaveProcess();
    
    DeathRespawnManager = NewObject<UPS_PlayerDeathRespawnManager>(this, UPS_PlayerDeathRespawnManager::StaticClass());
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    DeathRespawnManager->RespawnPlayer(PlayerController);
}