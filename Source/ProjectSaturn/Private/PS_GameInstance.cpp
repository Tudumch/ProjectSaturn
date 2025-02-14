// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_GameInstance.h"

#include "Systems/LoadSaveSystem/PS_LoadSaveManager.h"


void UPS_GameInstance::Init()
{
    Super::Init();
    
    LoadSaveManager = GetWorld()->SpawnActor<APS_LoadSaveManager>();
}
