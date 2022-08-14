// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TurretAIController.h"
#include "SciFiCombat/Public/Character/Turret.h"
#include "Monster/MonsterBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SciFiCombat/Public/GameMode/SciFiCombatGameMode.h"
#include "Kismet/GameplayStatics.h"



ATurretAIController::ATurretAIController()
{
	behavior_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	blackboard_component = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
}

void ATurretAIController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(behavior_tree.Get()))
	{
		RunBehaviorTree(behavior_tree.Get());
		behavior_tree_component->StartTree(*behavior_tree.Get());
	}
}

void ATurretAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetControlledTurret(Cast<ATurret>(InPawn));
	controlled_turret->SetTurretAIController(this);

	if (IsValid(Blackboard.Get()) && IsValid(behavior_tree.Get()))
	{
		Blackboard->InitializeBlackboard(*behavior_tree.Get()->BlackboardAsset.Get());
	}
}

void ATurretAIController::ChangeTurretState(int32 state)
{
	GetBlackboardComponent()->SetValueAsInt(FName("TurretState"), state);
}

void ATurretAIController::AttackMonster()
{
	controlled_turret->Attack();
}
