// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/JumpUpper.h"

#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

void AJumpUpper::OnSphereOverlap(
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
			ability->JumpUp(jump_z_vel, jump_up_time);
		}
	}

	Destroy();
}

void AJumpUpper::Destroyed()
{
	if (upper_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			upper_effect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	if (upper_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			upper_sound,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	Super::Destroyed();
}
