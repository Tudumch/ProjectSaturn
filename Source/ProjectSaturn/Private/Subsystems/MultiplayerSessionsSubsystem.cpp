// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MultiplayerSessionsSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void PrintString(const FString& Str)
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, Str);
}

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get()) // Steam or something
    {
        FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
        PrintString("Current Online Subsystem: " + SubsystemName);

        SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
        }
    }
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
}

void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
    if (ServerName.IsEmpty())
    {
        PrintString("Server name cannot be empty!");
        return;
    }

    FName MySessionName = FName("Co-op Adventure Session Name");

    if (FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(MySessionName))
    {
        FString Msg = FString::Printf(TEXT("Session with name %s already exists, destroying it."), *MySessionName.ToString());
        PrintString(Msg);
        CreateServerAfterDestroy = true;
        DestroyServerName = ServerName;
        SessionInterface->DestroySession(MySessionName);
        return;
    }
    
    FOnlineSessionSettings SessionSettings;

    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = false;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bAllowJoinViaPresence = true;
    
    bool bIsLAN = false;
    if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
        bIsLAN = true;
    SessionSettings.bIsLANMatch = bIsLAN;

    SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
    PrintString("Find server " + ServerName);
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool WasSuccessful)
{
    if (WasSuccessful)
        GetWorld()->ServerTravel(TestMapPath);
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool WasSuccessful)
{
    if (CreateServerAfterDestroy)
    {
        CreateServerAfterDestroy = false;
        CreateServer(DestroyServerName);
    }
}
