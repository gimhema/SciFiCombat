// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BarrierSkillObject.h"
#include "Components/BoxComponent.h"
#include "Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "SciFiCombat/Public/Weapon/MonsterProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

ABarrierSkillObject::ABarrierSkillObject()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	skill_area = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill Area"));
	skill_area->SetupAttachment(RootComponent);
}

void ABarrierSkillObject::BeginPlay()
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
	skill_area->OnComponentBeginOverlap.AddDynamic(this, &ABarrierSkillObject::OnBoxOverlap);

	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), destroy_delay, false);
}

void ABarrierSkillObject::OnBoxOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult
)
{
	bool monster_bullet_valid = IsValid(Cast<AMonsterProjectileBase>(OtherActor));

	FVector effect_location = OtherActor->GetActorLocation();
	FRotator effect_rotation = OtherActor->GetActorRotation();

	if (use_niagara)
	{
		if (defense_effect_niagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				defense_effect_niagara,
				effect_location,
				effect_rotation);
		}
	}
	else
	{
		if (defense_effect_particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				this,
				defense_effect_particle,
				effect_location,
				effect_rotation
			);
		}
	}

	if (monster_bullet_valid)
	{
		OtherActor->Destroy();
	}
}