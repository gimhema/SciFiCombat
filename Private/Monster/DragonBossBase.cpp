// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/DragonBossBase.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "Monster/AI/DragonBossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SciFiCombat/Public/Weapon/ProjectileBase.h"
#include "Animation/AnimationAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "SciFiCombat/Public/Monster/AttackObj/MonsterAttackObjBase.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
//#include "TimerManager.h"


ADragonBossBase::ADragonBossBase()
{
	//SetRootComponent(GetMesh());

	fire_muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("FireMuzzle"));
	fire_muzzle->SetupAttachment(GetMesh(), FName(TEXT("bite_socket")));

	linear_fire_muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("LinearFireMuzzle"));
	linear_fire_muzzle->SetupAttachment(RootComponent);

	bite_point = CreateDefaultSubobject<USceneComponent>(TEXT("BitePoint"));
	bite_point->SetupAttachment(GetMesh(), FName(TEXT("bite_socket")));

	attack_obj_point = CreateDefaultSubobject<USceneComponent>(TEXT("AttckPoint"));
	attack_obj_point->SetupAttachment(RootComponent);

	spawn_radial_force_component = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	spawn_radial_force_component->SetupAttachment(RootComponent);
}

void ADragonBossBase::BeginPlay()
{
	Super::BeginPlay();
	//LinearFireStart();
}

void ADragonBossBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADragonBossBase::SleepDown()
{

}

void ADragonBossBase::WakeUp()
{
	// �÷��̾ ������ ���� �� �����
	// BP���� ȣ����

	
	// �ִϸ��̼� ����
	PlayDragonMontage(dragon_wakeup_montage);
	//GetMesh()->PlayAnimation(dragon_wakeup, false);
	BlurEffect();
	// �����
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			is_wakeup = true;
		}), dragon_wakeup_montage->GetPlayLength(), false);

}


void ADragonBossBase::SpitFireBall()
{
	if (target_player)
	{
		
		
		FVector fire_start = fire_muzzle->GetComponentLocation();
		FVector fire_end = target_player->GetActorLocation();

		FRotator fire_rot = fire_muzzle->GetComponentRotation();

		

		//GetMesh()->PlayAnimation(dragon_spit_fire, false);
		PlayDragonMontage(dragon_spit_fire_montage);
		FTimerHandle delay_handle;
		GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
			{
				//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
				can_do_action = true;
			}), dragon_spit_fire_montage->GetPlayLength(), false);
		;

		UWorld* world = GetWorld();
		if (world)
		{
			world->SpawnActor<AProjectileBase>(
				dragon_fireball,
				fire_start,
				fire_rot
				);
		}
	}

}

void ADragonBossBase::NormalAttack()
{
	// �ִϸ��̼� ����
	//GetMesh()->PlayAnimation(dragon_normal_attack, false);
	PlayDragonMontage(dragon_normal_attack_montage);
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// �ִϸ��̼� ��� ����
			//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			can_do_action = true;
		}), dragon_normal_attack_montage->GetPlayLength(), true);

	// ������Ʈ ����
	FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);
	UWorld* world = GetWorld();
	if (world)
	{
		world->SpawnActor<AMonsterAttackObjBase>(
			monster_attack_obj,
			attack_obj_point->GetComponentLocation(),
			spawn_rot
			);
	}
}

void ADragonBossBase::SmashAttack()
{
	// �ִϸ��̼� ����
	//GetMesh()->PlayAnimation(dragon_smash_attack, false);
	PlayDragonMontage(dragon_smash_attack_montage);
	
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// �ִϸ��̼� ��� ����
			//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			can_do_action = true;
		}), dragon_smash_attack_montage->GetPlayLength(), true);

	// ������Ʈ ����
	FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);
	UWorld* world = GetWorld();
	if (world)
	{
		world->SpawnActor<AMonsterAttackObjBase>(
			monster_attack_obj,
			attack_obj_point->GetComponentLocation(),
			spawn_rot
			);
	}
}

void ADragonBossBase::Absorb()
{
	// �ִϸ��̼� ����
	//GetMesh()->PlayAnimation(dragon_smash_attack, false);
	//spawn_radial_force_component->ImpulseStrength = absorb_power;
	BlurEffect();
	PlayDragonMontage(dragon_absorb_montage);

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// �ִϸ��̼� ��� ����
			//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			can_do_action = true;
		}), dragon_absorb_montage->GetPlayLength(), true);

	// ������Ʈ ����
	spawn_radial_force_component->FireImpulse();

}

void ADragonBossBase::BiteAttack()
{
	// �ִϸ��̼� ����
	//GetMesh()->PlayAnimation(dragon_smash_attack, false);
	//spawn_radial_force_component->ImpulseStrength = bite_impulse_power;
	PlayDragonMontage(dragon_bite_attack_montage);

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// �ִϸ��̼� ��� ����
			//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			can_do_action = true;
		}), dragon_bite_attack_montage->GetPlayLength(), true);

	// ������Ʈ ����
	FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);
	UWorld* world = GetWorld();
	if (world)
	{
		world->SpawnActor<AMonsterAttackObjBase>(
			bite_attack_obj,
			GetActorLocation(),
			spawn_rot
			);
	}

	// �켱 Ÿ�� �÷��̾���� �Ÿ��� ���
	//if (target_player)
	//{
	//	float _dist = FVector::Distance(target_player->GetActorLocation(), GetActorLocation());
	//	// �Ÿ��� Bite �ȿ� ������
	//	if (_dist <= bite_range)
	//	{
	//		//GetMesh()->PlayAnimation(dragon_bite_attack_montage, false);
	//		PlayDragonMontage(dragon_bite_attack_montage);
	//		// Bite Socket�� �÷��̾ ����ġ
	//		FRotator origin_player_rotation = target_player->GetActorRotation();
	//		target_player->AttachToComponent(bite_point, FAttachmentTransformRules::KeepWorldTransform);
	//		//target_player->AttachToComponent(bite_point, FAttachmentTransformRules::KeepRelativeTransform);
	//		FTimerHandle delay_handle;
	//		GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
	//			{
	//				// �ִϸ��̼� ��� ����
	//				// �ִϸ��̼� ������ ����ġ
	//				//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//				target_player->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//				target_player->SetActorRotation(origin_player_rotation);
	//				can_do_action = true;
	//				//target_player->DetachRootComponentFromParent();
	//			}), dragon_bite_attack_montage->GetPlayLength(), true);

	//	}
	//}
}

void ADragonBossBase::SpreadFireBall()
{


}

void ADragonBossBase::FireMultiple()
{
	UWorld* world = GetWorld();
	if (world)
	{
		for (int i = 0; i < num_spread_fire; i++)
		{
			float x_rand = UKismetMathLibrary::RandomFloatInRange(-500.f, 500.f);
			float y_rand = UKismetMathLibrary::RandomFloatInRange(-500.f, 500.f);
			FVector target_loc = target_player->GetActorLocation();
			FVector fire_start = fire_muzzle->GetComponentLocation();
			FVector fire_end = FVector(target_loc.X + x_rand, target_loc.Y + y_rand, target_loc.Z);

			FRotator fire_rot = (fire_start - fire_end).Rotation();

			world->SpawnActor<AProjectileBase>(
				dragon_spread_fireball,
				fire_start,
				fire_rot
				);
		}
	}
}

void ADragonBossBase::LinearFireBall()
{
	if (linear_fire_mod == false)
	{
		linear_fire_mod = true;
	}
	else
	{
		linear_fire_mod = false;
	}
}

void ADragonBossBase::LinearFireStart()
{
	// Ÿ�̸� �����Լ�
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// Write Game Logic . . . .
			LinearFireLoop();
		}), linear_fire_delay, true);
}

void ADragonBossBase::LinearFireLoop()
{
	if (linear_fire_mod == true)
	{
		// ���̾ �߻�
		FVector fire_start = linear_fire_muzzle->GetComponentLocation();
		FRotator fire_rot = linear_fire_muzzle->GetComponentRotation();

		UWorld* world = GetWorld();
		if (world)
		{
			world->SpawnActor<AProjectileBase>(
				dragon_linear_fireball,
				fire_start,
				fire_rot
				);
		}
	}
}


void ADragonBossBase::PlayDragonMontage(class UAnimMontage* montage)
{
	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (anim_instance)
	{
		anim_instance->Montage_Play(montage);
	}
}

void ADragonBossBase::ChageMovementMode()
{

}

// depercated
void ADragonBossBase::FollowTargetRotation()
{
	//if (is_wakeup)
	//{
	//	if()
	//}
}
