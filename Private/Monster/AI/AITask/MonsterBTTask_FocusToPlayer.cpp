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
	// 해당 태스크를 진행한다는것은 이미 Monster가 Player를 인식했음을 의미한다.
	AMonsterAIControllerBase* monster_controller = Cast<AMonsterAIControllerBase>(OwnerComp.GetAIOwner());
	bool target_valid = monster_controller->GetControlledMonster()->TargetPlayerValid();
	if (target_valid)
	{
		// Target Valid가 유효한 상태에서만 동작함.
		ACombatCharacter* _target = monster_controller->GetControlledMonster()->target_player;
		// Target Player의 Location을 가져온다.
		FVector target_player_loc = monster_controller->GetControlledMonster()->target_player->GetActorLocation();

		//monster_controller->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, target_player_loc);
		monster_controller->GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, _target);
		// 그냥 몸통 돌리지말고 블랙보드에만 저장한다...
		// 다음 태스크에서는 선택된 블랙보드키로 MoveTo를 진행한다.
	}
	// 해당 Location을 Base Locatio으로 지정하고 Succeed로 돌린다.
	return EBTNodeResult::Succeeded;
}

FString UMonsterBTTask_FocusToPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
