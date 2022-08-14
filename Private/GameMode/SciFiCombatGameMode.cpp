// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SciFiCombatGameMode.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "SciFiCombat/Public/PlayerState/SciFiCombatPlayerState.h"
#include "SciFiCombat/Public/GameState/SciFiCombatGameState.h"
#include "SciFiCombat/Public/ItemSpawner/DefenseObj.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ASciFiCombatGameMode::ASciFiCombatGameMode()
{
	bDelayedStart = true;
}

void ASciFiCombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	game_start_time = GetWorld()->GetTimeSeconds();
}

void ASciFiCombatGameMode::InitializePlayerIFF()
{
	int32 _iff = 0;

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		ASciFiCombatPlayerController* match_player_controller = Cast< ASciFiCombatPlayerController>(*it);
		if (match_player_controller)
		{
			match_player_controller->SetPlayerIFF(_iff);
			_iff = _iff + 1;
		}
	}
}

void ASciFiCombatGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		ASciFiCombatPlayerController* match_player_controller = Cast< ASciFiCombatPlayerController>(*it);
		if (match_player_controller)
		{
			match_player_controller->OnMatchStateSet(MatchState);
		}
	}
}


void ASciFiCombatGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		game_count_down_time = warmup_time - GetWorld()->GetTimeSeconds() + game_start_time;
		if (game_count_down_time <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		game_count_down_time = warmup_time + game_time_limit - GetWorld()->GetTimeSeconds() + game_start_time;
		if (game_count_down_time <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		game_count_down_time = cooldown_time + warmup_time + game_time_limit - GetWorld()->GetTimeSeconds() + game_start_time;
		if (game_count_down_time <= 0.f)
		{
			RestartGame();
			// UWorld* World = GetWorld();
			// if (World)
			// {
			// 	bUseSeamlessTravel = true;
			// 	World->ServerTravel(FString("/Game/Maps/CombatMap?listen"));
			// }
		}

	}
}

void ASciFiCombatGameMode::InitializeDefenseLocation(FVector _loc)
{
	defense_location = _loc;
}

void ASciFiCombatGameMode::PlayerEliminated(class ACombatCharacter* ElimmedCharacter,
	class ASciFiCombatPlayerController* VictimController,
	class ASciFiCombatPlayerController* AttackerController)
{
	ASciFiCombatPlayerState* attacker_player_state = AttackerController ? Cast<ASciFiCombatPlayerState>(AttackerController->PlayerState) : nullptr;
	ASciFiCombatPlayerState* victim_player_state = VictimController ? Cast<ASciFiCombatPlayerState>(VictimController->PlayerState) : nullptr;

	ASciFiCombatGameState* game_state = GetGameState<ASciFiCombatGameState>();

	if (attacker_player_state && attacker_player_state != victim_player_state && game_state)
	{
		attacker_player_state->AddToScore(1.f);
		game_state->UpdateScoreRanking(attacker_player_state);
	}

	if (victim_player_state)
	{
		victim_player_state->AddToDeathCount(1);
	}

	if (ElimmedCharacter)
	{
		ElimmedCharacter->CallEliminate();
	}
}

void ASciFiCombatGameMode::RequestRespawn(
	class ACharacter* ElimmedCharacter, AController* ElimmedController
)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), actors);
		int32 selection = FMath::RandRange(0, actors.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, actors[selection]);
	}
}