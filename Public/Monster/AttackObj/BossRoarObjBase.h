// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/AttackObj/MonsterAttackObjBase.h"
#include "BossRoarObjBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ABossRoarObjBase : public AMonsterAttackObjBase
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
	float slow_delay = 10.0f;
	UPROPERTY(EditAnywhere)
	float slow_force = 100.f;
};
