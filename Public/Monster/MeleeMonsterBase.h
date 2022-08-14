// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "MeleeMonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AMeleeMonsterBase : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	AMeleeMonsterBase();
public:
	virtual void Attack() override;

	UFUNCTION()
	void ResetAnimMode(float delay);

	UFUNCTION()
	void SpawnDamageObject();
public:
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* swing_anim_sequence1;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* swing_anim_sequence2;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* swing_anim_sequence3;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float anim_delay = 1.3f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonsterAttackObjBase> monster_attack_obj;

public:
	// Attack RPC
	UFUNCTION(Server, Reliable)
	void ServerAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastAttack();
};
