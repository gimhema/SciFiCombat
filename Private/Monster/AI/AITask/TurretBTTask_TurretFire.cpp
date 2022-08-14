// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/AITask/TurretBTTask_TurretFire.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SciFiCombat/Public/Monster/AI/MonsterAIControllerBase.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/Character/Turret.h"
#include "SciFiCombat/Public/Character/TurretAIController.h"




UTurretBTTask_TurretFire::UTurretBTTask_TurretFire()
{
	NodeName = TEXT("TurretFire");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(
		UTurretBTTask_TurretFire, BlackboardKey
	));
}

EBTNodeResult::Type UTurretBTTask_TurretFire::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	ATurretAIController* turret_controller = Cast<ATurretAIController>(OwnerComp.GetAIOwner());
	turret_controller->AttackMonster();
	//// �� �½�ũ�� ����ȴٴ� ���� �̹� �÷��̾ ���� ���ݹݰ濡 �������� �ǹ��Ѵ�.
	//ATurretAIController* turret_controller = Cast<ATurretAIController>(OwnerComp.GetAIOwner());
	//bool target_valid = turret_controller->GetControlledTurret()->TargetMonsterValid();
	//if (target_valid)
	//{
	//	// Monster Controller���� �����̺�Ʈ�� �����Ѵ�.
	//	turret_controller->AttackMonster();
	//}
	return EBTNodeResult::Succeeded;
}

FString UTurretBTTask_TurretFire::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}

