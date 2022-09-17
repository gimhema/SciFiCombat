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
	// AI Controller에서 Static하게 지정된 Location을 가져온다.
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
	// 해당 Location을 Base Locatio으로 지정하고 Succeed로 돌린다.
}

FString UMonsterBTTask_FindBaseLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
