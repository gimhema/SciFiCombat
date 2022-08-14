// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/AITask/MonserBTTask_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UMonserBTTask_FindRandomLocation::UMonserBTTask_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(
		UMonserBTTask_FindRandomLocation, BlackboardKey
	));
}

EBTNodeResult::Type UMonserBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	FNavLocation location{};

	AAIController* aiController = OwnerComp.GetAIOwner();
	const APawn* aiPawn = aiController->GetPawn();

	const FVector origin = aiPawn->GetActorLocation();

	const UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (IsValid(navSystem) && navSystem->GetRandomPointInNavigableRadius(origin, serachradius, location))
	{
		aiController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName,
			location.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

	//return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FString UMonserBTTask_FindRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}