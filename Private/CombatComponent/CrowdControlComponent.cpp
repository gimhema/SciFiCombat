// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent/CrowdControlComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"

// Sets default values for this component's properties
UCrowdControlComponent::UCrowdControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCrowdControlComponent::InitializeCCComponent(FVector _effect_spawn_loc)
{
	effect_spawn_loc = _effect_spawn_loc;
	effect_spawn_rot = FRotator(0.f, 0.f, 0.f);
}

// Called when the game starts
void UCrowdControlComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCrowdControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// Stun
void UCrowdControlComponent::CallStun(float cc_delay)
{
	ServerStun(cc_delay);
}

void UCrowdControlComponent::ServerStun_Implementation(float cc_delay)
{
	MultiCastStun(cc_delay);
}

void UCrowdControlComponent::MultiCastStun_Implementation(float cc_delay)
{
	Stun(cc_delay);
}

void UCrowdControlComponent::Stun(float cc_delay)
{
	// 이펙트 실행
	if (use_niagara_stun)
	{
		SpawnCrowdControlEffectNiagara(stun_effect_niagara, effect_spawn_loc, effect_spawn_rot);
	}
	else
	{
		SpawnCrowdControlEffect(stun_effect_particle, effect_spawn_loc, effect_spawn_rot);
	}

	SetIsCrowdControlled(true);

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// Write Game Logic . . . .
			SetIsCrowdControlled(false);
		}), cc_delay, false);
}


// AirBorne
void UCrowdControlComponent::CallAirborne(float cc_delay, float _force)
{
	ServerAirborne(cc_delay, _force);
}
void UCrowdControlComponent::ServerAirborne_Implementation(float cc_delay, float _force)
{
	MultiCastAirborne(cc_delay, _force);
}

void UCrowdControlComponent::MultiCastAirborne_Implementation(float cc_delay, float _force)
{
	Airborne(cc_delay, _force);
}

void UCrowdControlComponent::Airborne(float cc_delay, float _force)
{
	// 이펙트 실행
	if (use_niagara_airborne)
	{
		SpawnCrowdControlEffectNiagara(airborne_effect_niagara, effect_spawn_loc, effect_spawn_rot);
	}
	else
	{
		SpawnCrowdControlEffect(airborne_effect_particle, effect_spawn_loc, effect_spawn_rot);
	}

	// 공중에 뜸
	float current_jump_z = Cast<ACharacter>(GetOwner())->GetCharacterMovement()->JumpZVelocity;
	//Cast<ACharacter>(GetOwner())->GetCharacterMovement()->JumpZVelocity = _force;
	//Cast<ACharacter>(GetOwner())->Jump();

	FVector double_jump_vec(0.f, 0.f, _force);
	Cast<ACharacter>(GetOwner())->LaunchCharacter(double_jump_vec, false, false);

	 
	// 행동불능
	SetIsCrowdControlled(true);

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// 원상복구
			SetIsCrowdControlled(false);
			if(Cast<ACharacter>(GetOwner()))
			{
				Cast<ACharacter>(GetOwner())->GetCharacterMovement()->JumpZVelocity = current_jump_z;
			}
		}), cc_delay, false);
}

// Slow
void UCrowdControlComponent::CallSlow(float cc_delay, float slow_speed)
{
	ServerSlow(cc_delay, slow_speed);
}

void UCrowdControlComponent::ServerSlow_Implementation(float cc_delay, float slow_speed)
{
	MultiCastSlow(cc_delay, slow_speed);
}

void UCrowdControlComponent::MultiCastSlow_Implementation(float cc_delay, float slow_speed)
{
	Slow(cc_delay, slow_speed);
}

void UCrowdControlComponent::Slow(float cc_delay, float slow_speed)
{
	// 이펙트 실행
	if (use_niagara_slow)
	{
		SpawnCrowdControlEffectNiagara(slow_effect_niagara, effect_spawn_loc, effect_spawn_rot);
	}
	else
	{
		SpawnCrowdControlEffect(slow_effect_particle, effect_spawn_loc, effect_spawn_rot);
	}

	// 속도 감소
	float origin_speed = Cast<ACharacter>(GetOwner())->GetCharacterMovement()->MaxWalkSpeed;
	Cast<ACharacter>(GetOwner())->GetCharacterMovement()->MaxWalkSpeed = slow_speed;
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// 원상 복구
			Cast<ACharacter>(GetOwner())->GetCharacterMovement()->MaxWalkSpeed = origin_speed;
		}), cc_delay, false);
}


void UCrowdControlComponent::SpawnCrowdControlEffectNiagara(class UNiagaraSystem* niagara_effect,
	FVector spawn_loc, FRotator spawn_rot)
{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara_effect, spawn_loc,
			spawn_rot);
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), melee_hit_effect_niagara, spawn_loc, spawn_rot);
		UNiagaraFunctionLibrary::SpawnSystemAttached(niagara_effect, cc_marker, FName(TEXT("")), FVector(0.f, 0.f, 0.f),
			FRotator(0.f, 0.f, 0.f), EAttachLocation::Type::KeepWorldPosition, false, false);
}

void UCrowdControlComponent::SpawnCrowdControlEffect(UParticleSystem* effect,
	FVector spawn_loc, FRotator spawn_rot)
{
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		effect,
		spawn_loc,
		spawn_rot);
}

