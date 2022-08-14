// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/HealthPack.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

AHealthPack::AHealthPack()
{
	bReplicates = true;

}


void AHealthPack::OnSphereOverlap(
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
			ability->HealCharacter(heal_power, heal_time);
		}
	}
	Destroy();
}

void AHealthPack::Destroyed()
{
	if (heal_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			heal_effect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	if (heal_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			heal_sound,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	Super::Destroyed();
}

