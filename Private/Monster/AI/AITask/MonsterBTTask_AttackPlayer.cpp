// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/AITask/MonsterBTTask_AttackPlayer.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SciFiCombat/Public/Monster/AI/MonsterAIControllerBase.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"

UMonsterBTTask_AttackPlayer::UMonsterBTTask_AttackPlayer()
{
	NodeName = TEXT("AttackPlayer");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(
		UMonsterBTTask_AttackPlayer, BlackboardKey
	));
}

EBTNodeResult::Type UMonsterBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// �� �½�ũ�� ����ȴٴ� ���� �̹� �÷��̾ ���� ���ݹݰ濡 �������� �ǹ��Ѵ�.
	AMonsterAIControllerBase* monster_controller = Cast<AMonsterAIControllerBase>(OwnerComp.GetAIOwner());
	if (monster_controller->GetControlledMonster()->GetIsCCControlled() == false)
	{
		bool target_valid = monster_controller->GetControlledMonster()->TargetPlayerValid();
		if (target_valid)
		{
			// Monster Controller���� �����̺�Ʈ�� �����Ѵ�.
			monster_controller->AttackPlayer();
		}
	}
	return EBTNodeResult::Succeeded;
}

FString UMonsterBTTask_AttackPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}

