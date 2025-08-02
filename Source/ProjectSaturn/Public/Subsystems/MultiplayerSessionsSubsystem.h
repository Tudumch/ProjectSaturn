// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "MultiplayerSessionsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    UMultiplayerSessionsSubsystem();

    UPROPERTY()
    FString TestMapPath = "/Game/Maps/Cell/Level_Cell?listen";

    void Initialize(FSubsystemCollectionBase& Collection) override;
    void Deinitialize() override;

    UFUNCTION(BlueprintCallable)
    void CreateServer(FString ServerName);
    UFUNCTION(BlueprintCallable)
    void FindServer(FString ServerName);

protected:
    IOnlineSessionPtr SessionInterface;
    
    bool CreateServerAfterDestroy = false;
    FString DestroyServerName;
    FString ServerNameToFind = "";
    FName MySessionName = FName("Co-op Adventure Session Name");

    UFUNCTION()
    void OnCreateSessionComplete(FName SessionName, bool WasSuccessful);
	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool WasSuccessful);
    UFUNCTION()
    void OnFindSessionsComplete(bool WasSuccessful);
    
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    TSharedPtr<FOnlineSessionSearch> SessionSearch;
};


