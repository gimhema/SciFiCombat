// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WaitingGameWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"
//#include "SciFiCombat/SciFiCombatType/SciFiCombatAbilityType.h"
#include "SciFiCombat/Public/Character/SummonsBeacon.h"

bool UWaitingGameWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
	player_combat_controller = Cast<ASciFiCombatPlayerController>(player_controller);

	if (select_combat_master)
	{
		select_combat_master->OnClicked.AddDynamic(this, &UWaitingGameWidget::OnClickedSelectCombatMaster);
	}
	if (select_magician)
	{
		select_magician->OnClicked.AddDynamic(this, &UWaitingGameWidget::OnClickedSelectMagician);
	}
	if (select_guardian)
	{
		select_guardian->OnClicked.AddDynamic(this, &UWaitingGameWidget::OnClickedSelectGuardian);
	}

	return true;
}


void UWaitingGameWidget::OnClickedSelectCombatMaster()
{
	ServerSelectCombatMaster();
}

void UWaitingGameWidget::OnClickedSelectMagician()
{
	ServerSelectMagician();
}

void UWaitingGameWidget::OnClickedSelectGuardian()
{
	ServerSelectGuardian();
}

void UWaitingGameWidget::ServerSelectCombatMaster_Implementation()
{
	MultiCastSelectCombatMaster();
}
void UWaitingGameWidget::MultiCastSelectCombatMaster_Implementation()
{
	if (player_combat_controller)
	{
		player_combat_controller->selected_character_class = ECombatCharacterClass::ECC_CombatMaster;
	}
}
void UWaitingGameWidget::ServerSelectMagician_Implementation()
{
	MultiCastSelectMagician();
}
void UWaitingGameWidget::MultiCastSelectMagician_Implementation()
{
	if (player_combat_controller)
	{
		player_combat_controller->selected_character_class = ECombatCharacterClass::ECC_Magician;
	}
}
void UWaitingGameWidget::ServerSelectGuardian_Implementation()
{
	MultiCastSelectGuardian();
}
void UWaitingGameWidget::MultiCastSelectGuardian_Implementation()
{
	if (player_combat_controller)
	{
		player_combat_controller->selected_character_class = ECombatCharacterClass::ECC_Guardian;
	}
}


