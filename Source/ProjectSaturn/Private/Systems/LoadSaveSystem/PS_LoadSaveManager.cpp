// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/LoadSaveSystem/PS_LoadSaveManager.h"

#include "Components/PS_EnergyComponent.h"
#include "Components/PS_HealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/LoadSaveSystem/PS_SaveGame.h"

void UPS_LoadSaveManager::Save()
{
    if (!SaveGameObject)
    {
        SaveGameObject = Cast<UPS_SaveGame>(UGameplayStatics::CreateSaveGameObject(UPS_SaveGame::StaticClass()));
    }

    const ACharacter* Character = GetWorld()->GetGameInstance()->GetPrimaryPlayerController()->GetCharacter();
    if (!Character) return;

    SaveGameObject->PlayerTransform = Character->GetRootComponent()->GetComponentTransform();
    SaveGameObject->PlayerCurrentEnergy = Character->GetComponentByClass<UPS_EnergyComponent>()->GetCurrentEnergy();
    SaveGameObject->PlayerCurrentHealth = Character->GetComponentByClass<UPS_HealthComponent>()->GetCurrentHealth();
    
    UGameplayStatics::SaveGameToSlot(SaveGameObject, "MainSlot", 0);
    
    UE_LOG(LogTemp, Warning, TEXT("Game saved!"));
}

void UPS_LoadSaveManager::Load()
{
    SaveGameObject = Cast<UPS_SaveGame>(UGameplayStatics::LoadGameFromSlot("MainSlot", 0));
    if (!SaveGameObject) return;

    const ACharacter* Character = GetWorld()->GetGameInstance()->GetPrimaryPlayerController()->GetCharacter();
    if (!Character) return;

    Character->GetRootComponent()->SetWorldTransform(SaveGameObject->PlayerTransform);
    
    if (UPS_EnergyComponent* EnergyComponent = Character->GetComponentByClass<UPS_EnergyComponent>())
        EnergyComponent->SetCurrentEnergy(SaveGameObject->PlayerCurrentEnergy);
    if (UPS_HealthComponent* HealthComponent = Character->GetComponentByClass<UPS_HealthComponent>())
        HealthComponent->SetCurrentHealth(SaveGameObject->PlayerCurrentHealth);
    
    UE_LOG(LogTemp, Warning, TEXT("Game Loaded!"));
}

void UPS_LoadSaveManager::InitializeSaveProcess()
{
    FTimerHandle LoadingDelayHandle;
    GetWorld()->GetTimerManager().SetTimer(LoadingDelayHandle, this, &ThisClass::Load, 0.01, false); // delay for spawn all actors

    // Saving game every second
    FTimerHandle ConstantSavingProcessHandle;
    GetWorld()->GetTimerManager().SetTimer(ConstantSavingProcessHandle, this, &ThisClass::Save, 1, true); 
}