// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SubSkillSelectWidget.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "Components/Button.h"
#include "SciFiCombat/Public/HUD/SciFiCombatPlayerHUD.h"
#include "SciFiCombat/Public/HUD/PlayerStatusOverlay.h"
#include "Components/Image.h"
// _Implementation


bool USubSkillSelectWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
	player_combat_controller = Cast<ASciFiCombatPlayerController>(player_controller);

	//ACombatCharacter* player_combat_character = Cast<ACombatCharacter>(player_combat_controller->GetPawn());

	//if (player_combat_character && player_combat_character->ability_component)
	//{
	//	ECombatCharacterClass _selected_class = player_combat_character->selected_character_class;
	//	switch (_selected_class)
	//	{
	//		case ECombatCharacterClass::ECC_CombatMaster:
	//			main_skill_texture = player_combat_character->ability_component->teleport_ui_texture;
	//			break;
	//		case ECombatCharacterClass::ECC_Guardian:
	//			main_skill_texture = player_combat_character->ability_component->damage_immune_ui_texture;
	//			break;
	//		case ECombatCharacterClass::ECC_Magician:
	//			main_skill_texture = player_combat_character->ability_component->barrier_ui_texture;
	//			break;
	//	}
	//}

	if (subskill_select_blackhole)
	{
		subskill_select_blackhole->OnClicked.AddDynamic(this, &USubSkillSelectWidget::OnClickedSubskillSelectBlackHole);
	}
	if (subskill_select_impulse)
	{
		subskill_select_impulse->OnClicked.AddDynamic(this, &USubSkillSelectWidget::OnClickedSubskillSelectImpulse);
	}
	if (subskill_select_booster)
	{
		subskill_select_booster->OnClicked.AddDynamic(this, &USubSkillSelectWidget::OnClickedSubskillSelectBooster);
	}
	if (subskill_select_jumpup)
	{
		subskill_select_jumpup->OnClicked.AddDynamic(this, &USubSkillSelectWidget::OnClickedSubskillSelectJumpup);
	}
	if (subskill_select_heal)
	{
		subskill_select_heal->OnClicked.AddDynamic(this, &USubSkillSelectWidget::OnClickedSubskillSelectHeal);
	}

	return true;
}

// BlackHole
void USubSkillSelectWidget::OnClickedSubskillSelectBlackHole()
{
	APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
	player_combat_controller = Cast<ASciFiCombatPlayerController>(player_controller);
	ACombatCharacter* player_combat_character = Cast<ACombatCharacter>(player_combat_controller->GetPawn());
	//player_combat_controller->player_hud->status_overlay->sub_skill_texture = player_combat_character->sub_skill_blackhole_texture;
	player_combat_controller->player_hud->status_overlay->sub_skill_image->SetBrushFromTexture(player_combat_character->sub_skill_blackhole_texture);
	ServerSubskillSelectBlackHole();
}

void USubSkillSelectWidget::ServerSubskillSelectBlackHole_Implementation()
{
	MultiCastSubskillSelectBlackHole();
}

void USubSkillSelectWidget::MultiCastSubskillSelectBlackHole_Implementation()
{
	Cast<ACombatCharacter>(player_combat_controller->GetPawn())->SetAvailableSubAbility("BlackHole");
}

// Impulse
void USubSkillSelectWidget::OnClickedSubskillSelectImpulse()
{
	APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
	player_combat_controller = Cast<ASciFiCombatPlayerController>(player_controller);
	ACombatCharacter* player_combat_character = Cast<ACombatCharacter>(player_combat_controller->GetPawn());
	//player_combat_controller->player_hud->status_overlay->sub_skill_texture = player_combat_character->sub_skill_impulse_texture;
	player_combat_controller->player_hud->status_overlay->sub_skill_image->SetBrushFromTexture(player_combat_character->sub_skill_impulse_texture);
	ServerSubskillSelectImpulse();
}

void USubSkillSelectWidget::ServerSubskillSelectImpulse_Implementation()
{
	MultiCastSubskillSelectImpulse();
}

void USubSkillSelectWidget::MultiCastSubskillSelectImpulse_Implementation()
{
	Cast<ACombatCharacter>(player_combat_controller->GetPawn())->SetAvailableSubAbility("Impulse");
}

// Booster
void USubSkillSelectWidget::OnClickedSubskillSelectBooster()
{
	APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
	player_combat_controller = Cast<ASciFiCombatPlayerController>(player_controller);
	ACombatCharacter* player_combat_character = Cast<ACombatCharacter>(player_combat_controller->GetPawn());
	player_combat_controller->player_hud->status_overlay->sub_skill_image->SetBrushFromTexture(player_combat_character->sub_skill_speedbooster_texture);

	ServerSubskillSelectBooster();
}

void USubSkillSelectWidget::ServerSubskillSelectBooster_Implementation()
{
	MultiCastSubskillSelectBooster();
}

void USubSkillSelectWidget::MultiCastSubskillSelectBooster_Implementation()
{
	Cast<ACombatCharacter>(player_combat_controller->GetPawn())->SetAvailableSubAbility("SpeedBooster");
}

// Jump Up
void USubSkillSelectWidget::OnClickedSubskillSelectJumpup()
{
	APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
	player_combat_controller = Cast<ASciFiCombatPlayerController>(player_controller);
	ACombatCharacter* player_combat_character = Cast<ACombatCharacter>(player_combat_controller->GetPawn());
	//player_combat_controller->player_hud->status_overlay->sub_skill_texture = player_combat_character->sub_skill_jumpup_texture;
	player_combat_controller->player_hud->status_overlay->sub_skill_image->SetBrushFromTexture(player_combat_character->sub_skill_jumpup_texture);
	ServerSubskillSelectJumpup();
}

void USubSkillSelectWidget::ServerSubskillSelectJumpup_Implementation()
{
	MultiCastSubskillSelectJumpup();
}

void USubSkillSelectWidget::MultiCastSubskillSelectJumpup_Implementation()
{
	Cast<ACombatCharacter>(player_combat_controller->GetPawn())->SetAvailableSubAbility("IncreaseJumpUp");
}

// Heal
void USubSkillSelectWidget::OnClickedSubskillSelectHeal()
{
	APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
	player_combat_controller = Cast<ASciFiCombatPlayerController>(player_controller);
	ACombatCharacter* player_combat_character = Cast<ACombatCharacter>(player_combat_controller->GetPawn());
	//player_combat_controller->player_hud->status_overlay->sub_skill_texture = player_combat_character->sub_skill_heal_texture;
	player_combat_controller->player_hud->status_overlay->sub_skill_image->SetBrushFromTexture(player_combat_character->sub_skill_heal_texture);
	ServerSubskillSelectHeal();
}

void USubSkillSelectWidget::ServerSubskillSelectHeal_Implementation()
{
	MultiCastSubskillSelectHeal();
}

void USubSkillSelectWidget::MultiCastSubskillSelectHeal_Implementation()
{
	Cast<ACombatCharacter>(player_combat_controller->GetPawn())->SetAvailableSubAbility("SpawnHealObject");
}
