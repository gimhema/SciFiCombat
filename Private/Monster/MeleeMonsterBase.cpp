// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MeleeMonsterBase.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SciFiCombat/Public/Monster/AttackObj/MonsterAttackObjBase.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"

AMeleeMonsterBase::AMeleeMonsterBase()
{

}

void AMeleeMonsterBase::ServerAttack_Implementation()
{
	MultiCastAttack();
}

void AMeleeMonsterBase::MultiCastAttack_Implementation()
{
	// 스피드 0
	ZeroMaxWalkSpeed();
	// 주사위
	int32 dice_min = 0;
	int32 dice_max = 2;
	int32 dice = FMath::RandRange(dice_min, dice_max);
	// switch(주사위 결과)

	switch (dice)
	{
	case 0:
		// 주사위 결과에 따라서 애니메이션 재생하고
		GetMesh()->PlayAnimation(swing_anim_sequence1, false);
		// 히트박스 오브젝트 소환
		SpawnDamageObject();
		break;
	case 1:
		GetMesh()->PlayAnimation(swing_anim_sequence2, false);
		SpawnDamageObject();
		break;
	case 2:
		GetMesh()->PlayAnimation(swing_anim_sequence3, false);
		SpawnDamageObject();
		break;
	}

	// 딜레이 이후 스피드 원상복귀, 애니메이션 모드 복구
	ResetAnimMode(anim_delay);
}

void AMeleeMonsterBase::Attack()
{
	Super::Attack();
	ServerAttack();
}

void AMeleeMonsterBase::SpawnDamageObject()
{
	UWorld* world = GetWorld();

	FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);

	if (world)
	{
			world->SpawnActor<AMonsterAttackObjBase>(
			monster_attack_obj,
			GetActorLocation(),
			spawn_rot
			);
	}
}

void AMeleeMonsterBase::ResetAnimMode(float delay)
{
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			ResetMaxWalkSpeed();
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}), delay, false);
}