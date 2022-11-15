// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "DragonBossBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ADragonBossBase : public AMonsterBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

public:

	bool is_wakeup = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float roll_rate = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pitch_rate = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float yaw_rate = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float target_roll = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float target_pitch = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float target_yaw = 0.01f;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	class USceneComponent* fire_muzzle;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	class USceneComponent* bite_point;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	class USceneComponent* linear_fire_muzzle;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	class USceneComponent* attack_obj_point;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonsterAttackObjBase> monster_attack_obj;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonsterAttackObjBase> bite_attack_obj;

	bool linear_fire_mod = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float linear_fire_delay = 1.5f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> dragon_fireball;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> dragon_spread_fireball;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> dragon_linear_fireball;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> spread_fire_vec;

	UPROPERTY(EditAnywhere)
	class URadialForceComponent* spawn_radial_force_component;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bite_range = 500.f;
	UPROPERTY(EditAnywhere, Category = DragonAnimation)
		class UAnimMontage* dragon_normal_attack_montage;
	UPROPERTY(EditAnywhere, Category = DragonAnimation)
		class UAnimMontage* dragon_smash_attack_montage;
	UPROPERTY(EditAnywhere, Category = DragonAnimation)
		class UAnimMontage* dragon_bite_attack_montage;
	UPROPERTY(EditAnywhere, Category = DragonAnimation)
		class UAnimMontage* dragon_spit_fire_montage;
	UPROPERTY(EditAnywhere, Category = DragonAnimation)
		class UAnimMontage* dragon_absorb_montage;
	UPROPERTY(EditAnywhere, Category = DragonAnimation)
		class UAnimMontage* dragon_wakeup_montage;

	void PlayDragonMontage(class UAnimMontage* montage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragonImpulse)
		float absorb_power = -10000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragonImpulse)
		float bite_impulse_power = 10000.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 num_spread_fire = 10;

	bool can_do_action = true;
public:
	ADragonBossBase();

	UFUNCTION(BlueprintCallable)
	void SleepDown();

	UFUNCTION(BlueprintCallable)
	void WakeUp();

	// Hovering
	UFUNCTION(BlueprintCallable)
	void FollowTargetRotation();

	// Attack
	UFUNCTION(BlueprintCallable)
	void SpitFireBall();
	UFUNCTION(BlueprintCallable)
	void NormalAttack();
	UFUNCTION(BlueprintCallable)
	void SmashAttack();
	UFUNCTION(BlueprintCallable)
	void BiteAttack();
	UFUNCTION(BlueprintCallable)
	void Absorb();


	UFUNCTION(BlueprintCallable)
	void SpreadFireBall();
	void FireMultiple();

	UFUNCTION(BlueprintCallable)
	void LinearFireBall();

	void LinearFireStart();
	void LinearFireLoop();

	// MovementMode
	UFUNCTION(BlueprintCallable)
	void ChageMovementMode();

	UFUNCTION(BlueprintImplementableEvent)
	void BlurEffect();

};
