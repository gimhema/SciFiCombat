// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/SciFiCombatGameState.h"
#include "Net/UnrealNetwork.h"
#include "SciFiCombat/Public/PlayerState/SciFiCombatPlayerState.h"

ASciFiCombatGameState::ASciFiCombatGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("DataTable'/Game/Data/ItemDB.ItemDB'"));
	ItemDB = BP_ItemDB.Object;
}

void ASciFiCombatGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASciFiCombatGameState, game_winner);
}

UDataTable* ASciFiCombatGameState::GetItemDB() const
{
	return ItemDB;
}

void ASciFiCombatGameState::UpdateScoreRanking(class ASciFiCombatPlayerState* game_player)
{
	if (game_winner.Num() == 0)
	{
		game_winner.Add(game_player);
		winner_score = game_player->GetScore();
	}
	else if (game_player->GetScore() == winner_score)
	{
		game_winner.AddUnique(game_player);
	}
	else if (game_player->GetScore() > winner_score)
	{
		game_winner.Empty();
		game_winner.AddUnique(game_player);
		winner_score = game_player->GetScore();
	}
}