// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HealingSkillObject.h"
#include "Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"



AHealingSkillObject::AHealingSkillObject()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	skill_area = CreateDefaultSubobject<USphereComponent>(TEXT("Skill Area"));
	skill_area->SetupAttachment(RootComponent);
	skill_area->SetSphereRadius(150.f);
}

void AHealingSkillObject::BeginPlay()
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
	skill_area->OnComponentBeginOverlap.AddDynamic(this, &AHealingSkillObject::OnSphereOverlap);
	
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), destroy_delay, false);

}

void AHealingSkillObject::OnSphereOverlap(
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
		ACombatCharacter* overlaped_character = Cast<ACombatCharacter>(OtherActor);
		overlaped_character->ability_component->HealCharacter(heal_power, heal_time);
	}
}

