// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/AITask/MonsterBTTask_FocusToPlayer.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SciFiCombat/Public/Monster/AI/MonsterAIControllerBase.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"

UMonsterBTTask_FocusToPlayer::UMonsterBTTask_FocusToPlayer()
{
	NodeName = TEXT("FocusToPlayer");

	//BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(
	//	UMonsterBTTask_FocusToPlayer, BlackboardKey
	//));
	//TSubclassOf<UObject> target_player;

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMonsterBTTask_FocusToPlayer, BlackboardKey), ACombatCharacter::StaticClass());
}

EBTNodeResult::Type UMonsterBTTask_FocusToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// �ش� �½�ũ�� �����Ѵٴ°��� �̹� Monster�� Player�� �ν������� �ǹ��Ѵ�.
	AMonsterAIControllerBase* monster_controller = Cast<AMonsterAIControllerBase>(OwnerComp.GetAIOwner());
	bool target_valid = monster_controller->GetControlledMonster()->TargetPlayerValid();
	if (target_valid)
	{
		// Target Valid�� ��ȿ�� ���¿����� ������.
		ACombatCharacter* _target = monster_controller->GetControlledMonster()->target_player;
		// Target Player�� Location�� �����´�.
		FVector target_player_loc = monster_controller->GetControlledMonster()->target_player->GetActorLocation();

		//monster_controller->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, target_player_loc);
		monster_controller->GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, _target);
		// �׳� ���� ���������� �����忡�� �����Ѵ�...
		// ���� �½�ũ������ ���õ� ������Ű�� MoveTo�� �����Ѵ�.
	}
	// �ش� Location�� Base Locatio���� �����ϰ� Succeed�� ������.
	return EBTNodeResult::Succeeded;
}

FString UMonsterBTTask_FocusToPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
