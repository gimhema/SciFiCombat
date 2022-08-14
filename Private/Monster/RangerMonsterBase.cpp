// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RangerMonsterBase.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SciFiCombat/Public/Monster/AttackObj/MonsterAttackObjBase.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SceneComponent.h"
#include "SciFiCombat/Public/Weapon/ProjectileBase.h"


ARangerMonsterBase::ARangerMonsterBase()
{
	fire_muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("FireMuzzle"));
	fire_muzzle->SetupAttachment(RootComponent);

}

void ARangerMonsterBase::ServerAttack_Implementation()
{
	MultiCastAttack();
}

void ARangerMonsterBase::MultiCastAttack_Implementation()
{
	//
	GetMesh()->PlayAnimation(fire_anim_sequence, false);

	UWorld* world = GetWorld();

	if (world)
	{
		world->SpawnActor<AProjectileBase>(
			ranger_monster_bullet,
			fire_muzzle->GetComponentLocation(),
			fire_muzzle->GetComponentRotation()
			);
	}

	ResetAnimMode(anim_delay);
	//
}

void ARangerMonsterBase::Attack()
{
	Super::Attack();
	ServerAttack();
}

void ARangerMonsterBase::SpawnDamageObject()
{

}

void ARangerMonsterBase::ResetAnimMode(float delay)
{
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			ResetMaxWalkSpeed();
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}), delay, false);
}