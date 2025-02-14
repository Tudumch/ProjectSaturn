// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/LoadSaveSystem/PS_LoadSaveManager.h"

// Sets default values
APS_LoadSaveManager::APS_LoadSaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APS_LoadSaveManager::Save()
{
    // TODO: implement player character position saving 
    UE_LOG(LogTemp, Warning, TEXT("APS_LoadSaveManager::Save"));
}

void APS_LoadSaveManager::Load()
{
    // TODO: implement player character position loading 
    UE_LOG(LogTemp, Warning, TEXT("APS_LoadSaveManager::Load"));
}

// Called when the game starts or when spawned
void APS_LoadSaveManager::BeginPlay()
{
	Super::BeginPlay();
}

