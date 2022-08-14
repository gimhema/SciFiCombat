// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurretAIController.generated.h"

/**
 * 
 */

class UBehaviorTreeComponent;

UCLASS()
class SCIFICOMBAT_API ATurretAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATurretAIController();

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	UFUNCTION()
	void ChangeTurretState(int32 state);

public:
	void AttackMonster();
	
	UPROPERTY()
	class ATurret* controlled_turret;

	FORCEINLINE class ATurret* GetControlledTurret() { return controlled_turret; }
	FORCEINLINE void SetControlledTurret(class ATurret* _turret) { controlled_turret = _turret; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> behavior_tree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTreeComponent> behavior_tree_component;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBlackboardComponent> blackboard_component;

};
