// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIControllerBase.generated.h"

/**
 * 
 */
class UBehaviorTreeComponent;

UCLASS()
class SCIFICOMBAT_API AMonsterAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterAIControllerBase();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	UFUNCTION()
	void ChangeEnemyState(int32 state);

public:
	virtual void AttackPlayer();
	void InitializeBaseLocation();
	FORCEINLINE FVector GetBaseLocation() const { return base_location; }

	UPROPERTY()
	FVector base_location;

	UPROPERTY()
	class AMonsterBase* controlled_monster;

	FORCEINLINE class AMonsterBase* GetControlledMonster() { return controlled_monster; }
	FORCEINLINE void SetControlledMonster(class AMonsterBase* _monster) { controlled_monster = _monster; }
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> behavior_tree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTreeComponent> behavior_tree_component;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBlackboardComponent> blackboard_component;
};
