// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/AIDecorator/MonsterBTDecorator_CheckPlayer.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SciFiCombat/Public/Monster/AI/MonsterAIControllerBase.h"

UMonsterBTDecorator_CheckPlayer::UMonsterBTDecorator_CheckPlayer()
{

}

//EBlackboardNotificationResult UMonsterBTDecorator_CheckPlayer::OnBlackboardKeyValueChange(
//	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
//{
//	// 접근 방법
//	// AMonsterAIControllerBase* ai_controller = Cast<AMonsterAIControllerBase>(Blackboard.GetBrainComponent()->GetAIOwner());
//
//
//	return EBlackboardNotificationResult::ContinueObserving;
//}