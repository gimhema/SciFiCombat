// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SciFiCombatPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ASciFiCombatPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
private:

	UPROPERTY()
	class ACombatCharacter* character;
	UPROPERTY()
	class ASciFiCombatPlayerController* controller;

public:
	virtual void OnRep_Score() override;

	UFUNCTION()                
	virtual void OnRep_DeathCount();

	void AddToScore(float _score);
	void AddToDeathCount(int32 _death_count);
	
	UPROPERTY(ReplicatedUsing = OnRep_DeathCount)
	int32 death_count;
};
