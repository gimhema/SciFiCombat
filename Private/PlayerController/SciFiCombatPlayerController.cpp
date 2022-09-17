// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/SciFiCombatPlayerController.h"
#include "SciFiCombat/Public/HUD/SciFiCombatPlayerHUD.h"
#include "SciFiCombat/Public/HUD/PlayerStatusOverlay.h"
#include "Components/TextBlock.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "Net/UnrealNetwork.h"
#include "SciFiCombat/Public/GameMode/SciFiCombatGameMode.h"
#include "SciFiCombat/Public/HUD/WaitingGameWidget.h"
#include "SciFiCombat/Public/GameMode/SciFiCombatGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SciFiCombat/Public/CombatComponent/CombatComponent.h"
#include "SciFiCombat/Public/GameState/SciFiCombatGameState.h"
#include "SciFiCombat/Public/PlayerState/SciFiCombatPlayerState.h"
#include "SciFiCombat/Public/HUD/ReturnHome.h"
#include "SciFiCombat/Public/Character/CombatMasterBase.h"
#include "SciFiCombat/Public/Character/MagicianBase.h"
#include "SciFiCombat/Public/Character/GuardianBase.h"
#include "TimerManager.h"

void ASciFiCombatPlayerController::BeginPlay()
{
	Super::BeginPlay();



	player_hud = Cast<ASciFiCombatPlayerHUD>(GetHUD());
	CheckMatchState();
}

void ASciFiCombatPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASciFiCombatPlayerController, match_state);
	DOREPLIFETIME(ASciFiCombatPlayerController, player_IFF);
	DOREPLIFETIME(ASciFiCombatPlayerController, selected_character_class);
//	DOREPLIFETIME_CONDITION(UCombatComponent, carried_ammo, COND_OwnerOnly);
}

void ASciFiCombatPlayerController::SetHealthProgress(float health, float max_health)
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud && 
		player_hud->status_overlay && 
		player_hud->status_overlay->health_bar &&
		player_hud->status_overlay->health_text;

	if (hud_valid)
	{
		float __health_percent = health / max_health;
		// health_percent를 블루프린트 상으로 노출시켜야한다.
		player_hud->status_overlay->health_percent = __health_percent;
	}
	else
	{
		overlay_initialized = true;
		hud_health = health;
		hud_max_health = max_health;
	}
}

void ASciFiCombatPlayerController::SetSmashPowerProgress(float smash_power, float max_smash_power)
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud &&
		player_hud->status_overlay &&
		player_hud->status_overlay->smash_power_bar &&
		player_hud->status_overlay->smash_power_text;

	if (hud_valid)
	{
		float __sp_percent = smash_power / max_smash_power;
		player_hud->status_overlay->smash_power_percent = __sp_percent;
	}
	else
	{
		overlay_initialized = true;
		hud_smash_power = smash_power;
		hud_max_smash_power = max_smash_power;
	}
}

void ASciFiCombatPlayerController::SetManaProgress(float current_mana, float max_mana)
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud &&
		player_hud->status_overlay &&
		player_hud->status_overlay->mana_bar &&
		player_hud->status_overlay->mana_text;
	// Status Overlay에 mana_bar, mana_text, mana_percent 추가

	if (hud_valid)
	{
		float __mana_percent = current_mana / max_mana;
		player_hud->status_overlay->mana_percent = __mana_percent;
	}
	else
	{
		overlay_initialized = true;
		hud_mana = current_mana;
		hud_max_mana = max_mana;
	}
}

void ASciFiCombatPlayerController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);

	//switch (selected_character_class)
	//{
	//case ECombatCharacterClass::ECC_None:
	//	ACombatCharacter* combat_character = Cast<ACombatCharacter>(inPawn);
	//	if (combat_character)
	//	{
	//		SetHealthProgress(combat_character->GetCurrentHealth(), combat_character->GetMaxHealth());
	//		SetSmashPowerProgress(combat_character->GetCurrentSmashPower(), combat_character->GetMaxSmashPower());
	//	}
	//	break;
	//case ECombatCharacterClass::ECC_CombatMaster:
	//	break;
	//case ECombatCharacterClass::ECC_Magician:
	//	break;
	//case ECombatCharacterClass::ECC_Guardian:
	//	break;
	//}

	ACombatCharacter* combat_character = Cast<ACombatCharacter>(inPawn);
	if (combat_character)
	{
		SetHealthProgress(combat_character->GetCurrentHealth(), combat_character->GetMaxHealth());
		SetSmashPowerProgress(combat_character->GetCurrentSmashPower(), combat_character->GetMaxSmashPower());
		SetManaProgress(combat_character->GetCurrentMana(), combat_character->GetMaxMana());
	}
}

void ASciFiCombatPlayerController::SetScoreText(float score)
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud && player_hud->status_overlay && player_hud->status_overlay->score_amount;
	if (hud_valid)
	{
		FString _score = FString::Printf(TEXT("%d"), FMath::FloorToInt(score));
		player_hud->status_overlay->score_amount->SetText(FText::FromString(_score));
	}
	else
	{
		overlay_initialized = true;
		hud_score = score;
	}
}

void ASciFiCombatPlayerController::SetAbilityAlarmText(FString alarm_text)
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud && player_hud->status_overlay && player_hud->status_overlay->ability_alarm_text;
	if (hud_valid)
	{
		//FString alram = FString::Printf(TEXT("%d"), FMath::FloorToInt(score));
		player_hud->status_overlay->ability_alarm_text->SetText(FText::FromString(alarm_text));

		FTimerHandle delay_handle;
		GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
			{
				player_hud->status_overlay->ability_alarm_text->SetText(FText::FromString(""));
			}), 3.0, false);
	}
	else
	{
		overlay_initialized = true;
		hud_ability_alarm_string = alarm_text;
	}
}



void ASciFiCombatPlayerController::SetWeaponAmmoText(int32 ammo)
{
	// ammo가 29938일 경우 meele weapon으로 표기함 
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud &&
		player_hud->status_overlay &&
		player_hud->status_overlay->ammo_amount;
	if (hud_valid && Cast<ACombatCharacter>(GetPawn())->combat_component->equipped_weapon)
	{
		if (Cast<ACombatCharacter>(GetPawn())->combat_component->equipped_weapon->weapon_style == EWeaponStyle::WST_Melee)
		{
			FString _ammo = FString::Printf(TEXT("Melee Weapon"));
			player_hud->status_overlay->ammo_amount->SetText(FText::FromString(_ammo));
		}
		else
		{
			FString _ammo = FString::Printf(TEXT("AMMO : %d"), ammo);
			player_hud->status_overlay->ammo_amount->SetText(FText::FromString(_ammo));
		}
	}
}

void ASciFiCombatPlayerController::SetCarriedWeaponAmmoText(int32 ammo)
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud &&
		player_hud->status_overlay &&
		player_hud->status_overlay->carried_ammo_amount;
	if (hud_valid && Cast<ACombatCharacter>(GetPawn())->combat_component->equipped_weapon)
	{
		if (Cast<ACombatCharacter>(GetPawn())->combat_component->equipped_weapon->weapon_style == EWeaponStyle::WST_Melee)
		{
			FString _ammo = FString::Printf(TEXT(""));
			player_hud->status_overlay->carried_ammo_amount->SetText(FText::FromString(_ammo));
		}
		else
		{
			FString _ammo = FString::Printf(TEXT(" / %d"), ammo);
			player_hud->status_overlay->carried_ammo_amount->SetText(FText::FromString(_ammo));
		}

	}
}

void ASciFiCombatPlayerController::SetDeathCountText(int32 death)
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud && 
		player_hud->status_overlay && 
		player_hud->status_overlay->death_amount;
	if (hud_valid)
	{
		FString _death = FString::Printf(TEXT("%d"), death);
		player_hud->status_overlay->death_amount->SetText(FText::FromString(_death));
	}
	else
	{
		overlay_initialized = true;
		hud_death = death;
	}
}

void ASciFiCombatPlayerController::SetMatchCountDownText(float time)
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud &&
		player_hud->status_overlay &&
		player_hud->status_overlay->match_countdown_text;
	if (hud_valid)
	{
//		if (time < 0.f)
//		{
//			player_hud->status_overlay->match_countdown_text->SetText(FText());
//			return;
//		}

		int32 minutes = FMath::FloorToInt(time / 60.f);
		int32 sec = time - minutes * 60;

		FString _time = FString::Printf(TEXT("%02d:%02d"), minutes, sec);
		player_hud->status_overlay->match_countdown_text->SetText(FText::FromString(_time));
	}
}

void ASciFiCombatPlayerController::SetWaitingCountDownText(float _countdown)
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	bool hud_valid = player_hud &&
		player_hud->waiting_widget &&
		player_hud->waiting_widget->waiting_time_text;
	if (hud_valid)
	{
//		if (_countdown < 0.f)
//		{
//			player_hud->waiting_widget->waiting_time_text->SetText(FText());
//			return;
//		}
		int32 minutes = FMath::FloorToInt(_countdown / 60.f);
		int32 sec = _countdown - minutes * 60;
		FString _time = FString::Printf(TEXT("%02d:%02d"), minutes, sec);
		player_hud->waiting_widget->waiting_time_text->SetText(FText::FromString(_time));
	}
}

void ASciFiCombatPlayerController::SetHUDTime()
{
	float time_left = 0.f;
	if (match_state == MatchState::WaitingToStart)
	{
		time_left = wait_time - GetServerTime() + game_start_time;
	}
	else if (match_state == MatchState::InProgress)
	{
		time_left = wait_time + game_time_limit - GetServerTime() + game_start_time;
	}
	else if (match_state == MatchState::Cooldown)
	{
		time_left = cooldown_time + wait_time + game_time_limit - GetServerTime() + game_start_time;
	}

	uint32 sec_left = FMath::CeilToInt(time_left);
	if (countdown != sec_left)
	{
		if (match_state == MatchState::WaitingToStart || match_state == MatchState::Cooldown)
		{
			SetWaitingCountDownText(time_left);
		}
		if (match_state == MatchState::InProgress)
		{
			SetMatchCountDownText(time_left);
		}
		// SetMatchCountDownText(game_time_limit - GetServerTime());
	}
	countdown = sec_left;
}

void ASciFiCombatPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
}

void ASciFiCombatPlayerController::CheckTimeSync(float _delta)
{
	time_sync_running += _delta;
	if (IsLocalController() && time_sync_running > time_sync_param)
	{
		RequestServerTimeFromServer(GetWorld()->GetTimeSeconds());
		time_sync_running = 0.f;
	}
}

void ASciFiCombatPlayerController::CheckMatchState_Implementation()
{
	ASciFiCombatGameMode* game_mode = Cast<ASciFiCombatGameMode>(UGameplayStatics::GetGameMode(this));
	if (game_mode)
	{
		wait_time = game_mode->warmup_time;
		game_time_limit = game_mode->game_time_limit;
		cooldown_time = game_mode->cooldown_time;
		game_start_time = game_mode->game_start_time;
		match_state = game_mode->GetMatchState();
		ClientJointGame(
			match_state,
			wait_time,
			game_time_limit,
			cooldown_time,
			game_start_time
		);
	}
}

void ASciFiCombatPlayerController::ClientJointGame_Implementation(FName _match_state,
	float _wait_time, float _match, float _cool_down, float _game_start_time)
{
	wait_time = _wait_time;
	game_time_limit = _match;
	game_start_time = _game_start_time;
	match_state = _match_state;
	cooldown_time = _cool_down;
	OnMatchStateSet(match_state);
	if (player_hud && match_state == MatchState::WaitingToStart)
	{
		player_hud->AddWaitingWidget();
	}

}

void ASciFiCombatPlayerController::RequestServerTimeFromServer_Implementation(float client_request_time)
{
	float server_time = GetWorld()->GetTimeSeconds();
	ReportServerTimeFromClient(client_request_time, server_time);
}

void ASciFiCombatPlayerController::ReportServerTimeFromClient_Implementation(float client_report_time,
	float received_server_time)
{
	float round_trip_time = GetWorld()->GetTimeSeconds() - client_report_time;
	float current_server_time = received_server_time + (0.5f * round_trip_time);
	client_server_delta = current_server_time - GetWorld()->GetTimeSeconds();
}

float ASciFiCombatPlayerController::GetServerTime()
{
	return GetWorld()->GetTimeSeconds() + client_server_delta;
}

void ASciFiCombatPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		RequestServerTimeFromServer(GetWorld()->GetTimeSeconds());
	}
}

void ASciFiCombatPlayerController::OnMatchStateSet(FName state)
{
	match_state = state;
	if (match_state == MatchState::InProgress)
	{
		GameStartHandle();
	}
	else if (match_state == MatchState::Cooldown)
	{
		CoolDownHandle();
	}
}

void ASciFiCombatPlayerController::CoolDownHandle()
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	if (player_hud)
	{
		player_hud->status_overlay->RemoveFromParent();
		//
		bool hud_valid = player_hud->waiting_widget &&
			player_hud->waiting_widget->waiting_announcement_text &&
			player_hud->waiting_widget->info_text;

		if (hud_valid) // player_hud->waiting_widget
		{
			player_hud->waiting_widget->SetVisibility(ESlateVisibility::Visible);
			//
			FString announcement_text("New Game Wait . . . .");
			player_hud->waiting_widget->waiting_announcement_text->SetText(FText::FromString(announcement_text));

			ASciFiCombatGameState* game_state = Cast<ASciFiCombatGameState>(UGameplayStatics::GetGameState(this));
			ASciFiCombatPlayerState* game_player_state = GetPlayerState<ASciFiCombatPlayerState>();
			if (game_state && game_player_state)
			{
				TArray<ASciFiCombatPlayerState*> winner = game_state->game_winner;
				FString game_info_text;
				if (winner.Num() == 0)
				{
					game_info_text = FString("Game Draw");
				}
				else if (winner.Num() == 1 && winner[0] == game_player_state)
				{
					game_info_text = FString("Win ! ! ! !");
				}
				else if (winner.Num() == 1)
				{
					game_info_text = FString::Printf(TEXT("Game Winner : \n%s"), *winner[0]->GetPlayerName());
				}
				else if (winner.Num() > 1)
				{
					game_info_text = FString::Printf(TEXT("Game Winners \n"));
					for (auto win_member : winner)
					{
						game_info_text.Append(FString::Printf(TEXT("%s\n"), *win_member->GetPlayerName()));
					}
				}
				player_hud->waiting_widget->info_text->SetText(FText::FromString(game_info_text));
			}
		}
	}
	ACombatCharacter* player_character = Cast<ACombatCharacter>(GetPawn());
	if (player_character && player_character->GetCombatComponent())
	{
		player_character->gameplay_disable_option = true;
		player_character->GetCombatComponent()->PressFireKey(false);
	}
}

void ASciFiCombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent == nullptr) return;

	InputComponent->BindAction("Quit", IE_Pressed, this, &ASciFiCombatPlayerController::ShowReturnToHome);
}

void ASciFiCombatPlayerController::ShowReturnToHome()
{
	if (return_to_home_widget == nullptr) return;
	if (return_to_home == nullptr)
	{
		return_to_home = CreateWidget<UReturnHome>(this, return_to_home_widget);
	}
	if (return_to_home)
	{
		is_return_to_home_open = !is_return_to_home_open;
		if (is_return_to_home_open)
		{
			return_to_home->MenuSetup();
		}
		else
		{
			return_to_home->MenuTearDown();
		}
	}
}

void ASciFiCombatPlayerController::OnRep_MatchState()
{
	if (match_state == MatchState::InProgress)
	{
		GameStartHandle();
	}
	else if (match_state == MatchState::Cooldown)
	{
		CoolDownHandle();
	}

}

void ASciFiCombatPlayerController::GameStartHandle()
{
	player_hud = player_hud == nullptr ? Cast<ASciFiCombatPlayerHUD>(GetHUD()) : player_hud;
	if (player_hud)
	{
		player_hud->AddStatusOverlay();
		if (player_hud->waiting_widget)
		{
			player_hud->waiting_widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


void ASciFiCombatPlayerController::PollInit()
{
	if (status_overlay == nullptr)
	{
		if (player_hud && player_hud->status_overlay)
		{
			status_overlay = player_hud->status_overlay;
			if(status_overlay)
			{
				SetHealthProgress(hud_health, hud_max_health);
				SetScoreText(hud_score);
				SetDeathCountText(hud_death);
				SetSmashPowerProgress(hud_smash_power, hud_max_smash_power);
				SetManaProgress(hud_mana, hud_max_mana);
				SetAbilityAlarmText(hud_ability_alarm_string);
			}
		}
	}
}

void ASciFiCombatPlayerController::PossessSelectClass(APawn* inPawn)
{
	if (HasAuthority())
	{
		Possess(inPawn);
	}
	//ServerPossessSelectClass(inPawn);	
}

void ASciFiCombatPlayerController::ServerPossessSelectClass_Implementation(APawn* inPawn)
{
	//if (HasAuthority())
	//{
	//	Possess(inPawn);
	//}
	//MultiCastPossessSelectClass(inPawn);
}

void ASciFiCombatPlayerController::MultiCastPossessSelectClass_Implementation(APawn* inPawn)
{
	if (HasAuthority())
	{
		Possess(inPawn);
	}
}