// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_GameInstance.h"

#include "Systems/LoadSaveSystem/PS_LoadSaveManager.h"


void UPS_GameInstance::Init()
{
    Super::Init();
    
    LoadSaveManager = NewObject<UPS_LoadSaveManager>(this, UPS_LoadSaveManager::StaticClass(), NAME_None, RF_Standalone);
    LoadSaveManager->InitializeSaveProcess(); // TODO: how to restart the save-timer when the level changes?
}
