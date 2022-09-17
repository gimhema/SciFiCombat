// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlackHoleObject.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Monster/MonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "SciFiCombat/Public/CombatComponent/CrowdControlComponent.h"
#include "Character/CombatCharacter.h"
#include "Net/UnrealNetwork.h"


ABlackHoleObject::ABlackHoleObject()
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

void ABlackHoleObject::BeginPlay()
{
	Super::BeginPlay();

	if (skill_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			skill_sound,
			GetActorLocation()
		);
	}

	skill_area->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleObject::OnSphereOverlap);

	//FTimerHandle wait_handle;
	//GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
	//	{
	//		BurstImpulse();
	//	}), burst_delay, false);
}

void ABlackHoleObject::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult
)
{
	bool overlap_valid = IsValid(Cast<ACombatCharacter>(OtherActor));

	if (overlap_valid)
	{
		AMonsterBase* overlaped_character = Cast<AMonsterBase>(OtherActor);
		//overlaped_character->cc_component->CallSlow(slow_delay, slow_power);
		overlaped_character->cc_component->CallStun(slow_delay);
		//UGameplayStatics::ApplyRadialDamageWithFalloff(
		//	this,
		//	500.f ,
		//	10.f,
		//	GetActorLocation(),
		//	200.f,
		//	500.f,
		//	1.f,
		//	UDamageType::StaticClass(),
		//	TArray<AActor*>(),
		//	this,
		//	nullptr
		//);
	}
}


void ABlackHoleObject::BurstImpulse()
{
	ServerBurstImpulse();
}

void ABlackHoleObject::ServerBurstImpulse_Implementation()
{
	MultiCastBurstImpulse();
}

void ABlackHoleObject::MultiCastBurstImpulse_Implementation()
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
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), destroy_delay, false);

}

