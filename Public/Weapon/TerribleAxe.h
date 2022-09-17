// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/MeleeWeaponBase.h"
#include "TerribleAxe.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ATerribleAxe : public AMeleeWeaponBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* swing_anim_sequence1;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* swing_anim_sequence2;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* swing_anim_sequence3;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* smash_anim_sequence;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_force = 20000000.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_distance = 1000.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_launch_delay = 0.2;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float attackh_dash_force = 20000000.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float attackh_dash_distance = 1000.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float attack_dash_reset_delay1 = 0.4f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float attack_dash_reset_delay2 = 0.4f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float attack_dash_reset_delay3 = 0.4f;


public:
	virtual void DoMeleeAttack(int32 combo_idx) override;
	virtual void CCProcess(AActor* cc_target_actor) override;
	void PlaySwingAnimMontage(int32 combo_idx);
	virtual void MeleeSmashAttack() override;
	void AttackDash(float delay);
	void ResetAnimMode(float delay);
};
