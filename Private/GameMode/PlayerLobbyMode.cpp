// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PlayerLobbyMode.h"
#include "GameFramework/GameStateBase.h"

void APlayerLobbyMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 num_player = GameState.Get()->PlayerArray.Num();
	if (num_player == num_game_start_player)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/CombatMap?listen"));
		}
	}
}


