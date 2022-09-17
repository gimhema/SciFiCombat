// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "OnlineSessionSettings.h"

UCreateGameInstance::UCreateGameInstance()
{

}

void UCreateGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
				this, &UCreateGameInstance::OnCreateSessionComplete
			);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
				this, &UCreateGameInstance::OnFindSessionComplete
			);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
				this, &UCreateGameInstance::OnJoinSessionComplete
			);
		}
	}
}

void UCreateGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/Maps/CombatMap?listen");
		//CombatMap
	}
}

void UCreateGameInstance::OnFindSessionComplete(bool Succeeded)
{
	if (Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		if (SearchResults.Num())
		{
			SessionInterface->JoinSession(0, FName("Create Session"), SearchResults[0]);
		}
	}
}

void UCreateGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JointAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JointAddress);
		if (JointAddress != "")
		{
			PController->ClientTravel(JointAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UCreateGameInstance::CreateServer()
{
	//UE_LOG(LogTemp, Waring, TEXT("Create Server ! ! !"));
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL") ? false : true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;

	SessionInterface->CreateSession(0, FName("Create Session"), SessionSettings);
}

void UCreateGameInstance::JoinServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}
