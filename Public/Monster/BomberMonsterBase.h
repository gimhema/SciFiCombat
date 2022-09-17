// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "BomberMonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ABomberMonsterBase : public AMonsterBase
{
	GENERATED_BODY()
public:
	ABomberMonsterBase();
public:
	virtual void Attack() override;

	UFUNCTION()
	void ResetAnimMode(float delay);

	UFUNCTION()
	void SpawnDamageObject();
public:
	UPROPERTY(EditAnywhere)
	class URadialForceComponent* spawn_radial_force_component;


	UPROPERTY(EditAnywhere, Category = AttackOption)
	class UAnimationAsset* fire_anim_sequence;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	float anim_delay = 1.3f;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	float bomb_delay = 1.3f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> ranger_monster_bullet;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonsterAttackObjBase> monster_attack_obj;
public:
	// Attack RPC
	UFUNCTION(Server, Reliable)
	void ServerAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastAttack();
};
