// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MagicianBase.h"
#include "Character/AbilityObjectBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMagicianBase::AMagicianBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}


void AMagicianBase::PressAbility1()
{
	Super::PressAbility1();
	ServerUseAbility1();
}

void AMagicianBase::PressAbility2()
{
	Super::PressAbility2();
	ServerUseAbility2();
}

void AMagicianBase::PressAbility3()
{
	Super::PressAbility3();
	ServerUseAbility3();
}

void AMagicianBase::ServerUseAbility1_Implementation()
{
	MulticastUseAbility1();
}

void AMagicianBase::MulticastUseAbility1_Implementation()
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

void AMagicianBase::ServerUseAbility2_Implementation()
{
	MulticastUseAbility2();
}

void AMagicianBase::MulticastUseAbility2_Implementation()
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
			world->SpawnActor<AAbilityObjectBase>(
				ability2_object,
				spawn_location,
				spawn_rotation,
				spawn_params
				);
		}
	}

}

void AMagicianBase::ServerUseAbility3_Implementation()
{
	MulticastUseAbility3();
}

void AMagicianBase::MulticastUseAbility3_Implementation()
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



