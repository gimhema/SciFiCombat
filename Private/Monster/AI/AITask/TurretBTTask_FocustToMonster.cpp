// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/AITask/TurretBTTask_FocustToMonster.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SciFiCombat/Public/Monster/AI/MonsterAIControllerBase.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/Character/Turret.h"
#include "SciFiCombat/Public/Character/TurretAIController.h"

UTurretBTTask_FocustToMonster::UTurretBTTask_FocustToMonster()
{
	NodeName = TEXT("FocusToMonster");


	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UTurretBTTask_FocustToMonster, BlackboardKey), APawn::StaticClass());
}

EBTNodeResult::Type UTurretBTTask_FocustToMonster::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// �ش� �½�ũ�� �����Ѵٴ°��� �̹� Monster�� Player�� �ν������� �ǹ��Ѵ�.
	ATurretAIController* turret_controller = Cast<ATurretAIController>(OwnerComp.GetAIOwner());
	bool target_valid = turret_controller->GetControlledTurret()->TargetMonsterValid();
	if (target_valid)
	{
		// Target Valid�� ��ȿ�� ���¿����� ������.
		APawn* _target = turret_controller->GetControlledTurret()->target_monster;
		// Target Player�� Location�� �����´�.
		FVector target_player_loc = turret_controller->GetControlledTurret()->target_monster->GetActorLocation();

		//monster_controller->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, target_player_loc);
		turret_controller->GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, _target);
		// �׳� ���� ���������� �����忡�� �����Ѵ�...
		// ���� �½�ũ������ ���õ� ������Ű�� MoveTo�� �����Ѵ�.
	}
	// �ش� Location�� Base Locatio���� �����ϰ� Succeed�� ������.
	return EBTNodeResult::Succeeded;
}

FString UTurretBTTask_FocustToMonster::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}

