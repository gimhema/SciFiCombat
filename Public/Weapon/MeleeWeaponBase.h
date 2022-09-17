// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CombatWeapon.h"
#include "MeleeWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AMeleeWeaponBase : public ACombatWeapon
{
	GENERATED_BODY()
	
public:
	AMeleeWeaponBase();

	//virtual void Fire(const FVector& hit_target) override;
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	class UStaticMeshComponent* weapon_staic_mesh;

	UPROPERTY(VisibleAnyWhere, Category = MeleeAttackOption)
	class USphereComponent* melee_hit_area;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float melee_damage = 0.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float melee_damage_reset = 0.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_damage = 0.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_power_gain = 20.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_delay = 0.8;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float spawn_smash_obj_delay = 0.8;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	TSubclassOf<class ASmashDamageObjectDefault> smash_obj;

	UPROPERTY()
	FTimerHandle combo_reset_handle;
	UPROPERTY()
	FTimerHandle melee_attack_enable_handle;

	UPROPERTY()
	bool is_continue_combo = false;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float combo_reset_delay = 3.2f;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float smash_obj_sizeup_offset = 2.f;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	FVector melee_hit_area_reset_size;

	UPROPERTY(EditAnywhere, Category = MeleeAttackEffectOption)
	class USoundCue* melee_hit_sound;
	UPROPERTY(EditAnywhere, Category = MeleeAttackEffectOption)
	class UNiagaraSystem* melee_hit_effect_niagara;
	UPROPERTY(EditAnywhere, Category = MeleeAttackEffectOption)
	class UParticleSystem* melee_hit_effect;
	UPROPERTY(EditAnywhere, Category = MeleeAttackEffectOption)
	bool melee_hit_use_niagara = false;

	UPROPERTY(EditAnywhere, Category = CCOption)
	float cc_delay = 2.0f;
	UPROPERTY(EditAnywhere, Category = CCOption)
	float cc_force = 600.0f;
	UPROPERTY()
	bool cc_mode = false;

public:
	// Melee Method
	virtual void ComboProcess() override;
	UFUNCTION()
	virtual void CCProcess(AActor* cc_target_actor);
	UFUNCTION()
	virtual void DoMeleeAttack(int32 combo_idx);
	UFUNCTION()
	void MeleeAttackEnableHandleFunc();
	UFUNCTION()
	void MeleeAttackEnableReset();
	UFUNCTION()
	void MeleeAttackActivate();
	UFUNCTION()
	void ComboResetHandleFunc();
	UFUNCTION()
	void ComboResetHandleClear();
	UFUNCTION()
	void ComboReset();
	UFUNCTION()
	void SendMeleeDamage();
	UFUNCTION()
	void OverlapMeleeHitArea(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void SpawnSmashAttackObj(FVector spawn_loc, FRotator spawn_rot);

	UFUNCTION()
	void HitAreaSizeup();
	UFUNCTION()
	void HitAreaReset();

	UFUNCTION()
	void SpawnMeleeHitEffect(FVector spawn_loc);
	UFUNCTION(Server, Reliable)
	void ServerSpawnMeleeHitEffect(FVector spawn_loc);
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSpawnMeleeHitEffect(FVector spawn_loc);
};
