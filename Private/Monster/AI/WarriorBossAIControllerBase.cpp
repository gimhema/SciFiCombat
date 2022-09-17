// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/WarriorBossAIControllerBase.h"
#include "Monster/WarriorBossBase.h"

AWarriorBossAIControllerBase::AWarriorBossAIControllerBase()
{

}

void AWarriorBossAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeControlledBoss(Cast<AWarriorBossBase>(GetPawn()));
	//if (IsValid(behavior_tree.Get()))
	//{
	//	RunBehaviorTree(behavior_tree.Get());
	//	behavior_tree_component->StartTree(*behavior_tree.Get());
	//}
}

void AWarriorBossAIControllerBase::InitializeControlledBoss(class AWarriorBossBase* _boss)
{
	controlled_boss = _boss;
}
void AWarriorBossAIControllerBase::RandomDesicion()
{
	controlled_boss->RandomDesicion();
}
void AWarriorBossAIControllerBase::Roar()
{
	controlled_boss->Roar();
}
void AWarriorBossAIControllerBase::SelectTarget()
{
	controlled_boss->SelectTargetPlayer();
}
void AWarriorBossAIControllerBase::NormalAttack()
{
	controlled_boss->NormalAttack();
}
void AWarriorBossAIControllerBase::TeleportPlayers()
{
	controlled_boss->AbsorptionAndSwing();
}
void AWarriorBossAIControllerBase::SmashAttack()
{
	controlled_boss->SmashAttack();
}
void AWarriorBossAIControllerBase::Hold()
{
	controlled_boss->Hold();
}