// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AttackObj/BossRoarObjBase.h"
#include "Components/SphereComponent.h"
#include "Character/CombatCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Animation/AnimationAsset.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SciFiCombat/Public/CombatComponent/CrowdControlComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SciFiCombat/Public/Monster/AttackObj/MonsterAttackObjBase.h"

void ABossRoarObjBase::OverlapMeleeHitArea(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult)
{
	bool target_player_valid = IsValid(Cast<ACombatCharacter>(OtherActor));

	if (target_player_valid)
	{
		Cast<ACombatCharacter>(OtherActor)->cc_component->Slow(slow_delay, slow_force);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Player Not Valid"));
		return;
	}
}
