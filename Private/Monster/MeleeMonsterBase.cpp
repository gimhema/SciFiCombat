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
	// ���ǵ� 0
	ZeroMaxWalkSpeed();
	// �ֻ���
	int32 dice_min = 0;
	int32 dice_max = 2;
	int32 dice = FMath::RandRange(dice_min, dice_max);
	// switch(�ֻ��� ���)

	switch (dice)
	{
	case 0:
		// �ֻ��� ����� ���� �ִϸ��̼� ����ϰ�
		GetMesh()->PlayAnimation(swing_anim_sequence1, false);
		// ��Ʈ�ڽ� ������Ʈ ��ȯ
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

	// ������ ���� ���ǵ� ���󺹱�, �ִϸ��̼� ��� ����
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