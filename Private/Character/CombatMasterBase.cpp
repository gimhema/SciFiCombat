// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CombatMasterBase.h"
#include "Character/AbilityObjectBase.h"
#include "Kismet/GameplayStatics.h"
#include "Character/TeleportGrenadeObject.h"
#include "Net/UnrealNetwork.h"

ACombatMasterBase::ACombatMasterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ACombatMasterBase::PressAbility1()
{
	Super::PressAbility1();
	ServerUseAbility1();
}

void ACombatMasterBase::PressAbility2()
{
	Super::PressAbility2();
	ServerUseAbility2();
}

void ACombatMasterBase::PressAbility3()
{
	Super::PressAbility3();
	ServerUseAbility3();
}

void ACombatMasterBase::ServerUseAbility1_Implementation()
{
	MulticastUseAbility1();
}

void ACombatMasterBase::MulticastUseAbility1_Implementation()
{
	FVector spawn_location = ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rotation = ability_object_muzzle->GetComponentRotation();
	if (!HasAuthority()) return;
	if (ability1_object)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = nullptr;
		spawn_params.Instigator = this;
		UWorld* world = GetWorld();

		if (world)
		{
			world->SpawnActor<AAbilityObjectBase>(
				ability1_object,
				spawn_location,
				spawn_rotation,
				spawn_params
				);
		}
	}

}

void ACombatMasterBase::ServerUseAbility2_Implementation()
{
	MulticastUseAbility2();
}

void ACombatMasterBase::MulticastUseAbility2_Implementation()
{
	FVector spawn_location = ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rotation = ability_object_muzzle->GetComponentRotation();
	if (!HasAuthority()) return;
	if (ability2_object)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = nullptr;
		spawn_params.Instigator = this;
		UWorld* world = GetWorld();

		if (world)
		{
			ATeleportGrenadeObject* _teleport_granade = world->SpawnActor<ATeleportGrenadeObject>(
				ability2_object,
				spawn_location,
				spawn_rotation,
				spawn_params
				);
			_teleport_granade->teleport_character = this;
		}
	}

}

void ACombatMasterBase::ServerUseAbility3_Implementation()
{
	MulticastUseAbility3();
}

void ACombatMasterBase::MulticastUseAbility3_Implementation()
{
	FVector spawn_location = ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rotation = ability_object_muzzle->GetComponentRotation();
	if (!HasAuthority()) return;
	if (ability3_object)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = nullptr;
		spawn_params.Instigator = this;
		UWorld* world = GetWorld();

		if (world)
		{
			world->SpawnActor<AAbilityObjectBase>(
				ability3_object,
				spawn_location,
				spawn_rotation,
				spawn_params
				);
		}
	}

}