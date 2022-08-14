// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CombatSpear.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/CombatComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACombatSpear::DoMeleeAttack(int32 combo_idx)
{
	if (!is_can_melee_attack) return;
	PlaySwingAnimMontage(combo_idx);

	is_can_melee_attack = false;
	melee_hit_enable = true;

	melee_combo_count++;
	is_continue_combo = true;

	if (melee_combo_count >= melee_combo_count_max) { ComboReset(); }

	FTimerHandle _combo_reset_handle;
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			is_can_melee_attack = true;
			melee_hit_enable = false;
		}), melee_attack_enable_delay, false);


	if (!is_continue_combo)
	{
		GetWorld()->GetTimerManager().SetTimer(_combo_reset_handle, FTimerDelegate::CreateLambda([&]()
			{
				combo_idx = 0;
				is_continue_combo = false;
			}), combo_reset_delay, false);
	}
}

void ACombatSpear::PlaySwingAnimMontage(int32 combo_idx)
{
	if (weapon_owner_character)
	{
		switch (combo_idx)
		{
		case 0:
			weapon_owner_character->ForceMoveCharacter(attackh_dash_distance, attackh_dash_force);
			Cast<ACombatCharacter>(GetOwner())->GetMesh()->PlayAnimation(swing_anim_sequence1, false);
			ResetAnimMode(attack_dash_reset_delay1);
			break;
		case 1:
			weapon_owner_character->ForceMoveCharacter(attackh_dash_distance, attackh_dash_force);
			Cast<ACombatCharacter>(GetOwner())->GetMesh()->PlayAnimation(swing_anim_sequence2, false);
			ResetAnimMode(attack_dash_reset_delay2);
			break;
		case 2:
			weapon_owner_character->ForceMoveCharacter(attackh_dash_distance, attackh_dash_force);
			Cast<ACombatCharacter>(GetOwner())->GetMesh()->PlayAnimation(swing_anim_sequence3, false);
			ResetAnimMode(attack_dash_reset_delay3);
			break;
		case 3:
			weapon_owner_character->ForceMoveCharacter(attackh_dash_distance, attackh_dash_force);
			Cast<ACombatCharacter>(GetOwner())->GetMesh()->PlayAnimation(swing_anim_sequence4, false);
			ResetAnimMode(attack_dash_reset_delay4);
			break;
		case 4:
			weapon_owner_character->ForceMoveCharacter(attackh_dash_distance, attackh_dash_force);
			Cast<ACombatCharacter>(GetOwner())->GetMesh()->PlayAnimation(swing_anim_sequence5, false);
			ResetAnimMode(attack_dash_reset_delay5);
			break;
		}
	}
}

void ACombatSpear::AttackDash(float delay)
{
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			weapon_owner_character->ForceMoveCharacter(attackh_dash_force, attackh_dash_distance);
		}), delay, false);


}

void ACombatSpear::ResetAnimMode(float delay)
{
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			weapon_owner_character->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}), delay, false);
}

void ACombatSpear::MeleeSmashAttack()
{
	Super::MeleeSmashAttack();

	is_can_melee_attack = false;
	melee_hit_enable = true;

	melee_damage = smash_damage;
	//FVector smash_foward_velocity = UKismetMathLibrary::GetForwardVector(weapon_owner_character->GetActorRotation()) * smash_distance;
	weapon_owner_character->GetMesh()->PlayAnimation(smash_anim_sequence, false);

	FTimerHandle smash_handle;
	GetWorld()->GetTimerManager().SetTimer(smash_handle, FTimerDelegate::CreateLambda([&]()
		{
			// LaunchCharacter
			weapon_owner_character->ForceMoveCharacter(smash_distance, smash_force);
			//weapon_owner_character->GetCharacterMovement()->AddForce(smash_foward_velocity.GetSafeNormal(0.0001) * smash_force);
			// 여기에 코드를 치면 된다.
		}), smash_launch_delay, false); //반복도 여기서 추가 변수를 선언해 설정가능

	// Delay
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			is_can_melee_attack = true;
			melee_hit_enable = false;
			melee_damage = melee_damage_reset;
			weapon_owner_character->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			// 여기에 코드를 치면 된다.
		}), smash_delay, false); //반복도 여기서 추가 변수를 선언해 설정가능
	// Delay
}