// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/MonsterAIControllerBase.h"
#include "Monster/MonsterBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SciFiCombat/Public/GameMode/SciFiCombatGameMode.h"
#include "Kismet/GameplayStatics.h"

AMonsterAIControllerBase::AMonsterAIControllerBase()
{
	behavior_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	blackboard_component = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
}

void AMonsterAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(behavior_tree.Get()))
	{
		InitializeBaseLocation();
		RunBehaviorTree(behavior_tree.Get());
		behavior_tree_component->StartTree(*behavior_tree.Get());
	}
}

void AMonsterAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	SetControlledMonster(Cast<AMonsterBase>(InPawn));
	controlled_monster->SetMonsterAIController(this);
	
	if (IsValid(Blackboard.Get()) && IsValid(behavior_tree.Get()))
	{
		Blackboard->InitializeBlackboard(*behavior_tree.Get()->BlackboardAsset.Get());
	}
}

void AMonsterAIControllerBase::AttackPlayer()
{
	// GetActor 클래스를 통해서 Base Location을 지정한다.
	controlled_monster->Attack();
}

void AMonsterAIControllerBase::InitializeBaseLocation()
{
	// GetActor 클래스를 통해서 Base Location을 지정한다.
	ASciFiCombatGameMode* game_mode = Cast<ASciFiCombatGameMode>(UGameplayStatics::GetGameMode(this));
	if (game_mode)
	{
		base_location = game_mode->GetDefenseLocation();
	}
}

void AMonsterAIControllerBase::ChangeEnemyState(int32 state)
{
	//monster_controller->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, target_player_loc);
	GetBlackboardComponent()->SetValueAsInt(FName("EnemyState"), state);
}












