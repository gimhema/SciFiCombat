// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/AI/MonsterAIControllerBase.h"
#include "WarriorBossAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AWarriorBossAIControllerBase : public AMonsterAIControllerBase
{
	GENERATED_BODY()
public:
	AWarriorBossAIControllerBase();

protected:
	virtual void BeginPlay() override;

public:
	// Property

	UPROPERTY()
	class AWarriorBossBase* controlled_boss;

public:
	// Method
	UFUNCTION()
	void InitializeControlledBoss(class AWarriorBossBase* _boss);
	UFUNCTION()
	void RandomDesicion();
	UFUNCTION()
	void Roar();
	UFUNCTION()
	void SelectTarget();
	UFUNCTION()
	void NormalAttack();
	UFUNCTION()
	void TeleportPlayers();
	UFUNCTION()
	void SmashAttack();
	UFUNCTION()
	void Hold();
};
