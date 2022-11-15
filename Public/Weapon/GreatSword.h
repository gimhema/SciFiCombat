// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/MeleeWeaponBase.h"
#include "GreatSword.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AGreatSword : public AMeleeWeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimMontage* swing_montage1;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimMontage* swing_montage2;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimMontage* swing_montage3;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* swing_anim_sequence1;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* swing_anim_sequence2;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* swing_anim_sequence3;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* smash_anim_sequence;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* dash_smash_anim_sequence;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_force = 20000000.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_distance = 1000.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_launch_delay = 0.2;
public:
	virtual void DoMeleeAttack(int32 combo_idx) override;
	virtual void CCProcess(AActor* cc_target_actor) override;
	void PlaySwingAnimMontage(int32 combo_idx);
	virtual void MeleeSmashAttack() override;
	virtual void MeleeDashSmashAttack() override;
};
