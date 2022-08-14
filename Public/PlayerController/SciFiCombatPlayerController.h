// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
//#include "SciFiCombat/SciFiCombatType/SciFiCombatAbilityType.h"
#include "SciFiCombatPlayerController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECombatCharacterClass : uint8
{
	ECC_CombatMaster UMETA(DisplayName = "Combat Master"),
	ECC_Magician UMETA(DisplayName = "Magician"),
	ECC_Guardian UMETA(DisplayName = "Guardian"),
	ECC_None UMETA(DisplayName = "None")
};

UCLASS()
class SCIFICOMBAT_API ASciFiCombatPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	float game_start_time = 0.f;
	float game_time_limit = 0.f;
	float cooldown_time = 10.f;
	float wait_time = 0.f;
	uint32 countdown = 0;

	UPROPERTY(Replicated)
	int32 player_IFF = 0;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class ASciFiCombatPlayerHUD* player_hud;


	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName match_state;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY(Replicated)
	ECombatCharacterClass selected_character_class = ECombatCharacterClass::ECC_None;

	UPROPERTY()
	class UPlayerStatusOverlay* status_overlay;
	bool overlay_initialized = false;

	float hud_health;
	float hud_max_health;
	float hud_score;
	float hud_smash_power;
	float hud_max_smash_power;
	int32 hud_death;

public:
	void SetHealthProgress(float health, float max_health);
	void SetSmashPowerProgress(float smash_power, float max_smash_power);
	void SetScoreText(float score);
	void SetDeathCountText(int32 death);
	void SetWeaponAmmoText(int32 ammo);
	void SetCarriedWeaponAmmoText(int32 ammo);
	void SetMatchCountDownText(float time);
	void SetWaitingCountDownText(float _countdown);

	FORCEINLINE void SetPlayerIFF(int32 IFF) { player_IFF = IFF; }
	FORCEINLINE int32 GetPlayerIFF() { return player_IFF; }

	virtual void OnPossess(APawn* inPawn) override;
	virtual void Tick(float DeltaTime) override;

	virtual float GetServerTime();
	virtual void ReceivedPlayer() override;
	void OnMatchStateSet(FName state);
	void GameStartHandle();
	void CoolDownHandle();

	// Return to Home
	void ShowReturnToHome();

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<class UUserWidget> return_to_home_widget;

	UPROPERTY()
	class UReturnHome* return_to_home;

	bool is_return_to_home_open = false;
public:
	//Possess
	UFUNCTION()
	void PossessSelectClass(APawn* inPawn);
	UFUNCTION(Server, Reliable)
	void ServerPossessSelectClass(APawn* inPawn);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastPossessSelectClass(APawn* inPawn);
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void SetHUDTime();

	UFUNCTION(Server, Reliable)
	void RequestServerTimeFromServer(float client_request_time);

	UFUNCTION(Client, Reliable)
	void ReportServerTimeFromClient(float client_report_time, float received_server_time);

	float client_server_delta = 0.f;

	UPROPERTY(EditAnywhere, Category = Time)
	float time_sync_param = 5.f;

	float time_sync_running = 0.f;
	void CheckTimeSync(float _delta);

	UFUNCTION(Server, Reliable)
	void CheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJointGame(FName _match_state, float _wait_time, float _match, float _cool_down, float _game_start_time);

	void PollInit();
};








