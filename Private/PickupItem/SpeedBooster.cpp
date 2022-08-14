// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/SpeedBooster.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

void ASpeedBooster::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult
)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, isFromSweep, SweepResult);

	ACombatCharacter* combat_character = Cast<ACombatCharacter>(OtherActor);
	if (combat_character)
	{
		USciFiAbilityComponent* ability = combat_character->GetAbilityComponent();
		if (ability)
		{
			ability->BoostSpeed(booster_run_speed, booster_crouch_speed, booster_time);
		}
	}

	Destroy();
}

void ASpeedBooster::Destroyed()
{
	if (booster_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			booster_effect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	if (booster_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			booster_sound,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	Super::Destroyed();
}


