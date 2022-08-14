// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayerLobbyMode.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API APlayerLobbyMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	int32 num_game_start_player = 2;
};
