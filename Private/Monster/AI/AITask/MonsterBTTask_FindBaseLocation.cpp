// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/AITask/MonsterBTTask_FindBaseLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SciFiCombat/Public/Monster/AI/MonsterAIControllerBase.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"

UMonsterBTTask_FindBaseLocation::UMonsterBTTask_FindBaseLocation()
{
	NodeName = TEXT("FindBaseLocation");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(
		UMonsterBTTask_FindBaseLocation, BlackboardKey
	));
}

EBTNodeResult::Type UMonsterBTTask_FindBaseLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// AI Controller���� Static�ϰ� ������ Location�� �����´�.
	AMonsterAIControllerBase* monster_controller = Cast<AMonsterAIControllerBase>(OwnerComp.GetAIOwner());
	if (monster_controller->GetControlledMonster()->GetIsCCControlled() == false)
	{
		FVector base_location = Cast<AMonsterAIControllerBase>(OwnerComp.GetAIOwner())->base_location;

		AAIController* aiController = OwnerComp.GetAIOwner();

		aiController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, base_location);
		//aiController->GetBlackboardComponent()->SetValueAsBool

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	return EBTNodeResult::Succeeded;
	// �ش� Location�� Base Locatio���� �����ϰ� Succeed�� ������.
}

FString UMonsterBTTask_FindBaseLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
