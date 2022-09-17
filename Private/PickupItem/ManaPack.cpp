// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/ManaPack.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

AManaPack::AManaPack()
{
	bReplicates = true;

}


void AManaPack::OnSphereOverlap(
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
		combat_character->IncreaseMana(mana_power);
	}
	Destroy();
}

void AManaPack::Destroyed()
{
	if (mana_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			mana_effect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	if (mana_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			mana_sound,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	Super::Destroyed();
}
