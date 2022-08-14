// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/SciFiCombatPlayerState.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"
#include "Net/UnrealNetwork.h"
// #include "SciFiCombat/Public
// void ASciFiCombatPlayerState::MethodName()
// {

// }

void ASciFiCombatPlayerState::AddToScore(float _score)
{
	SetScore(GetScore() + _score);
	// Score += _score;
	character = character == nullptr ? Cast<ACombatCharacter>(GetPawn()) : character;
	if (character)
	{
		controller = controller == nullptr ? Cast<ASciFiCombatPlayerController>(character->Controller) : controller;
		if (controller)
		{
			// controller->SetScoreText(_score);
			controller->SetScoreText(GetScore());
		}
	}
}

void ASciFiCombatPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	character = character == nullptr ? Cast<ACombatCharacter>(GetPawn()) : character;
	if (character)
	{
		controller = controller == nullptr ? Cast<ASciFiCombatPlayerController>(character->Controller) : controller;
		if (controller)
		{
			controller->SetScoreText(GetScore());
		}
	}
}

void ASciFiCombatPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASciFiCombatPlayerState, death_count);
}

void ASciFiCombatPlayerState::AddToDeathCount(int32 _death_count)
{
	death_count += _death_count;
	character = character == nullptr ? Cast<ACombatCharacter>(GetPawn()) : character;
	if (character)
	{
		controller = controller == nullptr ? Cast<ASciFiCombatPlayerController>(character->Controller) : controller;
		if (controller)
		{
			controller->SetDeathCountText(death_count);
		}
	}
}

void ASciFiCombatPlayerState::OnRep_DeathCount()
{
	character = character == nullptr ? Cast<ACombatCharacter>(GetPawn()) : character;
	if (character)
	{
		controller = controller == nullptr ? Cast<ASciFiCombatPlayerController>(character->Controller) : controller;
		if (controller)
		{
			controller->SetDeathCountText(death_count);
		}
	}
}



