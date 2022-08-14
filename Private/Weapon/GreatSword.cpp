// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GreatSword.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/CombatComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"

void AGreatSword::DoMeleeAttack(int32 combo_idx)
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

void AGreatSword::PlaySwingAnimMontage(int32 combo_idx)
{
	if (weapon_owner_character)
	{
		switch (combo_idx)
		{
		case 0:
			//Cast<ACombatCharacter>(GetOwner())->GetMesh()->PlayAnimation(swing_anim_sequence1, false);
			weapon_owner_character->PlayCombatCharacterAnimMontage(swing_montage1);
			break;
		case 1:
			weapon_owner_character->PlayCombatCharacterAnimMontage(swing_montage2);		
			break;
		case 2:
			weapon_owner_character->PlayCombatCharacterAnimMontage(swing_montage3);
			break;
		}
	}
}

void AGreatSword::MeleeSmashAttack()
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

	// 이쯤에 투사체형 오브젝트를 추가한다.
	FTimerHandle spawn_obj_handle;
	FVector spawn_location = weapon_owner_character->ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rotation = weapon_owner_character->ability_object_muzzle->GetComponentRotation();
	GetWorld()->GetTimerManager().SetTimer(spawn_obj_handle, FTimerDelegate::CreateLambda([&]()
		{
			// LaunchCharacter
			FRotator rot = FRotator(0.f, 0.f, 0.f);
			SpawnSmashAttackObj(spawn_location, spawn_rotation);
			//weapon_owner_character->GetCharacterMovement()->AddForce(smash_foward_velocity.GetSafeNormal(0.0001) * smash_force);
			// 여기에 코드를 치면 된다.
		}), spawn_smash_obj_delay, false);

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