// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/AITask/BossBTTask_Hold.h"
#include "Monster/AI/WarriorBossAIControllerBase.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SciFiCombat/Public/Monster/AI/MonsterAIControllerBase.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "TimerManager.h"

UBossBTTask_Hold::UBossBTTask_Hold()
{
	NodeName = TEXT("Hold");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(
		UBossBTTask_Hold, BlackboardKey
	));
}

EBTNodeResult::Type UBossBTTask_Hold::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// 이 태스크가 진행된다는 것은 이미 플레이어가 적의 공격반경에 들어왔음을 의미한다.
	//AMonsterAIControllerBase* monster_controller = Cast<AMonsterAIControllerBase>(OwnerComp.GetAIOwner());
	AWarriorBossAIControllerBase* boss_controller = Cast<AWarriorBossAIControllerBase>(OwnerComp.GetAIOwner());
	boss_controller->Hold();

	// N초후 상태변경
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			boss_controller->GetBlackboardComponent()->SetValueAsInt(FName("BossState"), 0);
		}), phase_switch, false);
	return EBTNodeResult::Succeeded;
}

FString UBossBTTask_Hold::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}

