// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GuardianBase.h"
#include "Character/AbilityObjectBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AGuardianBase::AGuardianBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AGuardianBase::PressAbility1()
{
	Super::PressAbility1();
	ServerUseAbility1();
}

void AGuardianBase::PressAbility2()
{
	Super::PressAbility2();
	ServerUseAbility2();
}

void AGuardianBase::PressAbility3()
{
	Super::PressAbility3();
	ServerUseAbility3();
}

void AGuardianBase::ServerUseAbility1_Implementation()
{
	MulticastUseAbility1();
}

void AGuardianBase::MulticastUseAbility1_Implementation()
{

}

void AGuardianBase::ServerUseAbility2_Implementation()
{
	MulticastUseAbility2();
}

void AGuardianBase::MulticastUseAbility2_Implementation()
{

}

void AGuardianBase::ServerUseAbility3_Implementation()
{
	MulticastUseAbility3();
}

void AGuardianBase::MulticastUseAbility3_Implementation()
{

}