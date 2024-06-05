// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
    bCreateServerAfterDestroy = false;
    DestroyServerName = "";
    ServerNameToFind = "";
    MapName = "";
    MySessionName = FName("Co-op Adventure Session Name");
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        PrintString(OnlineSubsystem->GetSubsystemName().ToString());

        SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
        }        
    }    
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{

}

void UMultiplayerSessionsSubsystem::PrintString(const FString& TextToPrint)
{
    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TextToPrint);
    UE_LOG(LogTemp, Warning, TEXT("%s"), * TextToPrint); 
}

void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
    PrintString(ServerName);  
    if (ServerName.IsEmpty())
    {
        PrintString("Server Name is Empty");
        ServerCreateDel.Broadcast(false);
        return;
    }

    FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
    if (ExistingSession)
    {
        PrintString("Destroying");
        bCreateServerAfterDestroy = true;
        DestroyServerName = ServerName;
        SessionInterface->DestroySession(MySessionName);
        return;
    }    

    FOnlineSessionSettings MySessionSettings;

    MySessionSettings.bAllowJoinInProgress = true;
    MySessionSettings.bIsDedicated = false;
    MySessionSettings.bShouldAdvertise = true;
    MySessionSettings.NumPublicConnections = 2;
    MySessionSettings.bUseLobbiesIfAvailable = true;
    MySessionSettings.bUsesPresence = true;
    MySessionSettings.bAllowJoinViaPresence = true;
    MySessionSettings.bIsLANMatch = IsLAN();
    MySessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    SessionInterface->CreateSession(0, MySessionName, MySessionSettings);    
}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
    PrintString(ServerName);
    if (ServerName.IsEmpty())
    {
        PrintString("Server Name is Empty");
        ServerJoinDel.Broadcast(false);
        return;
    }

    SearchSettings = MakeShareable(new FOnlineSessionSearch());

    SearchSettings->bIsLanQuery = IsLAN();
    SearchSettings->MaxSearchResults = 9999;
    SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    ServerNameToFind = ServerName;

    SessionInterface->FindSessions(0, SearchSettings.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    PrintString(FString::Printf(TEXT("Session created %d"), bWasSuccessful));
    ServerCreateDel.Broadcast(bWasSuccessful);
    if (bWasSuccessful)
    {
        GetWorld()->ServerTravel(MapName + "?listen");
    }    
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    PrintString(FString::Printf(TEXT("Session destroyed %d"), bWasSuccessful));

    if (bCreateServerAfterDestroy)
    {
        bCreateServerAfterDestroy = false;
        CreateServer(DestroyServerName);
    }    
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    TArray<FOnlineSessionSearchResult> Results = SearchSettings->SearchResults; 
    FOnlineSessionSearchResult* CorrectResult = 0;

    if (!bWasSuccessful) 
    {
        ServerJoinDel.Broadcast(false);
        return;
    }
    if (ServerNameToFind.IsEmpty()) return;

    if (Results.Num() > 0)
    {
        PrintString(FString::Printf(TEXT("%d Sessions found"), Results.Num()));

        for (FOnlineSessionSearchResult Result : Results)
        {
            if (Result.IsValid())
            {
                FString ServerName = "No-name";
                Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

                if (ServerName.Equals(ServerNameToFind))
                {
                    CorrectResult = &Result;
                    PrintString(FString::Printf(TEXT("Found Server: %s"), * ServerName));
                    break;
                }                
            }            
        } 

        if (CorrectResult)
        {
            SessionInterface->JoinSession(0, MySessionName, * CorrectResult);
        }   
        else
        {
            PrintString(FString::Printf(TEXT("Couldn't Find Server: %s"), * ServerNameToFind));
            ServerJoinDel.Broadcast(false);
            ServerNameToFind = "";
        }
        
    }
    else
    {
        ServerJoinDel.Broadcast(false);
        PrintString("No Session Found");
    }
    
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        PrintString(FString::Printf(TEXT("Joined Successfully To Session: %s"), * SessionName.ToString()));
        ServerJoinDel.Broadcast(true);

        FString Address = "";
        bool bSuccess = SessionInterface->GetResolvedConnectString(MySessionName, Address);
        if (bSuccess)
        {
            PrintString(Address);
            APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
            if (PlayerController)
            {
                PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
            }
        }
        else
        {
            PrintString("GetResolvedConnectString = false");
            ServerJoinDel.Broadcast(false);
        }
    }
    else
    {
        PrintString("OnJoinSessionComplete failed");
        ServerJoinDel.Broadcast(false);
    }
    
}

void UMultiplayerSessionsSubsystem::SetMapName(FString NewName)
{
    MapName = NewName;
}

bool UMultiplayerSessionsSubsystem::IsLAN()
{
    return (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
}
