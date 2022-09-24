// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SciFiCombatGameMode.generated.h"

namespace MatchState
{
	extern SCIFICOMBAT_API const FName Cooldown;
}
/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ASciFiCombatGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASciFiCombatGameMode();
	//virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerEliminated(class ACombatCharacter* ElimmedCharacter,
		class ASciFiCombatPlayerController* VictimController,
		class ASciFiCombatPlayerController* AttackerController);

	virtual void RequestRespawn(
		class ACharacter* ElimmedCharacter, AController* ElimmedController
	);

	UPROPERTY(EditDefaultsOnly)
	float warmup_time = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float game_time_limit = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float cooldown_time = 10.f;

	UPROPERTY()
	FVector defense_location;

	UFUNCTION()
	void InitializeDefenseLocation(FVector _loc);

	UFUNCTION()
	FORCEINLINE FVector GetDefenseLocation() { return defense_location; }

	UFUNCTION()
	void InitializePlayerIFF();

	float game_start_time = 0.f;

protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
	//virtual void StartMatch() override;
private:
	float game_count_down_time = 0.f;
};
