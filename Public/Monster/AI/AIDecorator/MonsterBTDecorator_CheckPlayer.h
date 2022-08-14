// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "MonsterBTDecorator_CheckPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UMonsterBTDecorator_CheckPlayer : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UMonsterBTDecorator_CheckPlayer();

public:
	UPROPERTY()
	bool is_find_player = false;

public:
	//virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(
	//	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;

};
