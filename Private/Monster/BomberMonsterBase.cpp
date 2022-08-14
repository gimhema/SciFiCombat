// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BomberMonsterBase.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SciFiCombat/Public/Monster/AttackObj/MonsterAttackObjBase.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SceneComponent.h"
#include "SciFiCombat/Public/Weapon/ProjectileBase.h"


ABomberMonsterBase::ABomberMonsterBase()
{

}

void ABomberMonsterBase::ServerAttack_Implementation()
{
	MultiCastAttack();
}

void ABomberMonsterBase::MultiCastAttack_Implementation()
{
	//
	GetMesh()->PlayAnimation(fire_anim_sequence, false);

	UWorld* world = GetWorld();

	FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);

	if (world)
	{
		world->SpawnActor<AMonsterAttackObjBase>(
			monster_attack_obj,
			GetActorLocation() ,
			spawn_rot
			);
	}

	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), bomb_delay, false);
	
	//
}

void ABomberMonsterBase::Attack()
{
	Super::Attack();
	ServerAttack();
}

void ABomberMonsterBase::SpawnDamageObject()
{

}

void ABomberMonsterBase::ResetAnimMode(float delay)
{
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			ResetMaxWalkSpeed();
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}), delay, false);
}