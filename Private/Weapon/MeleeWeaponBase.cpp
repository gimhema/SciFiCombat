// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MeleeWeaponBase.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/Weapon/SmashDamageObjectDefault.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"

AMeleeWeaponBase::AMeleeWeaponBase()
{
	melee_hit_area = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeHitBox"));
	melee_hit_area->SetupAttachment(weapon_mesh);
	melee_hit_area->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	melee_hit_area->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	weapon_staic_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeStaticMesh"));
	weapon_staic_mesh->SetupAttachment(RootComponent);
	weapon_staic_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	weapon_staic_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//RootComponent
}


void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) // Same Meaning -> GetLocalRole() == ENetRole::ROLE_Authority
	{
		melee_hit_area->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		melee_hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		melee_hit_area->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OverlapMeleeHitArea);
	}
}

void AMeleeWeaponBase::ComboProcess()
{
	Super::ComboProcess();
	DoMeleeAttack(melee_combo_count);

	//if (melee_combo_count >= melee_combo_count_max) { ComboReset(); }
	//melee_combo_count = melee_combo_count + 1;
}

void AMeleeWeaponBase::DoMeleeAttack(int32 combo_idx)
{
	/*
	switch (combo_idx) {
	case 0:
		ComboResetHandleClear();
		PlayAnimation();
		MeleeAttackActivate();
		MeleeAttackEnableHandleFunc();
		break;
	case . . . :
	}
	*/
}

void AMeleeWeaponBase::SpawnSmashAttackObj(FVector spawn_loc, FRotator spawn_rot)
{
	if (!HasAuthority()) return;
	APawn* instigator = Cast<APawn>(GetOwner());
	if (smash_obj && instigator)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = GetOwner();
		spawn_params.Instigator = instigator;
		UWorld* world = GetWorld();
		if (world)
		{
			world->SpawnActor<ASmashDamageObjectDefault>(
				smash_obj,
				spawn_loc,
				spawn_rot,
				spawn_params
				);
		}
	}
}

void AMeleeWeaponBase::MeleeAttackEnableHandleFunc()
{
	GetWorldTimerManager().SetTimer(
		melee_attack_enable_handle,
		this,
		&AMeleeWeaponBase::MeleeAttackEnableReset,
		melee_combo_reset_delay
	);
}

void AMeleeWeaponBase::MeleeAttackEnableReset()
{
	is_can_melee_attack = true;
	melee_hit_enable = false;
	ComboResetHandleFunc();
}

void AMeleeWeaponBase::MeleeAttackActivate()
{
	is_can_melee_attack = false;
	melee_hit_enable = true;
}

void AMeleeWeaponBase::ComboResetHandleFunc()
{
	GetWorldTimerManager().SetTimer(
		combo_reset_handle,
		this,
		&AMeleeWeaponBase::ComboReset,
		melee_attack_enable_delay
	);
}

void AMeleeWeaponBase::ComboResetHandleClear()
{
	if (!GetWorldTimerManager().IsTimerPaused(combo_reset_handle))
	{
		GetWorldTimerManager().ClearTimer(combo_reset_handle);
	}
}

void AMeleeWeaponBase::ComboReset()
{
	melee_combo_count = 0;
}

void AMeleeWeaponBase::SendMeleeDamage()
{

}

void AMeleeWeaponBase::OverlapMeleeHitArea(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult)
{
	if (!weapon_owner_character) { return; }

	bool monster_valid = IsValid(Cast<AMonsterBase>(OtherActor));

	if (melee_hit_enable && monster_valid)
	{
		// melee_hit_enable이 활성화된 경우 : 좌클릭으로 공격을 수행
		// 닿은 액터에게 데미지를 전달
		ACharacter* owner_character = Cast<ACharacter>(GetOwner());
		if (owner_character)
		{
			AController* owner_controller = owner_character->Controller;
			if (owner_controller)
			{
				UGameplayStatics::ApplyDamage(OtherActor, melee_damage, owner_controller, this, UDamageType::StaticClass());
				weapon_owner_character->IncreaseSmashPower(smash_power_gain);
			}
		}
	}
	else
	{
		// melee_hit_enable이 비활성화된 경우 : 무기 미장착, 혹은 클릭을 수행하지않은 경우
		return;
	}
}

