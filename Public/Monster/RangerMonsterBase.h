// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "RangerMonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ARangerMonsterBase : public AMonsterBase
{
	GENERATED_BODY()

public:
	ARangerMonsterBase();
public:
	virtual void Attack() override;

	UFUNCTION()
	void ResetAnimMode(float delay);

	UFUNCTION()
	void SpawnDamageObject();
public:

	UPROPERTY(EditAnywhere, Category = AttackOption)
	class USceneComponent* fire_muzzle;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	class UAnimationAsset* fire_anim_sequence;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	float anim_delay = 1.3f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> ranger_monster_bullet;


public:
	// Attack RPC
	UFUNCTION(Server, Reliable)
	void ServerAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastAttack();

	
};
