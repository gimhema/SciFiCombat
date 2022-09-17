// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/AttackObj/MonsterAttackObjBase.h"
#include "BossSmashObjBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ABossSmashObjBase : public AMonsterAttackObjBase
{
	GENERATED_BODY()
public:
	virtual void OverlapMeleeHitArea(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere)
	float airborne_delay = 2.0f;
	UPROPERTY(EditAnywhere)
	float airborne_force = 300.f;
};
