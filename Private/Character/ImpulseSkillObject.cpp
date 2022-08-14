// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ImpulseSkillObject.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

AImpulseSkillObject::AImpulseSkillObject()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	hit_area = CreateDefaultSubobject<UBoxComponent>(TEXT("HitArea"));
	SetRootComponent(hit_area);
	hit_area->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	hit_area->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	hit_area->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	skill_area = CreateDefaultSubobject<USphereComponent>(TEXT("Skill Area"));
	skill_area->SetupAttachment(RootComponent);

	spawn_radial_force_component = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	spawn_radial_force_component->SetupAttachment(RootComponent);
}

void AImpulseSkillObject::BeginPlay()
{
	Super::BeginPlay();
	

	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			BurstImpulse();
		}), burst_delay, false);
}


void AImpulseSkillObject::BurstImpulse()
{
	ServerBurstImpulse();
}

void AImpulseSkillObject::ServerBurstImpulse_Implementation()
{
	MultiCastBurstImpulse();
}

void AImpulseSkillObject::MultiCastBurstImpulse_Implementation()
{
	if (skill_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			skill_sound,
			GetActorLocation()
		);
	}

	if (is_use_niagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), spawn_effect_niagara, GetActorLocation(),
			GetActorRotation());
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			spawn_effect_partice,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	spawn_radial_force_component->FireImpulse();

	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), destroy_delay, false);
	
}


