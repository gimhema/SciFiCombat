// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SciFiCombatGameState.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ASciFiCombatGameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void UpdateScoreRanking(class ASciFiCombatPlayerState* game_player);

	UPROPERTY(Replicated)
	TArray<class ASciFiCombatPlayerState*> game_winner;
private:
	float winner_score = 0.f;

};
