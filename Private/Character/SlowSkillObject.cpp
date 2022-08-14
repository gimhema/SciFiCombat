// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SlowSkillObject.h"
#include "Components/BoxComponent.h"
#include "Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "SciFiCombat/Public/Weapon/MonsterProjectileBase.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

ASlowSkillObject::ASlowSkillObject()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	skill_area = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill Area"));
	skill_area->SetupAttachment(RootComponent);
}

void ASlowSkillObject::BeginPlay()
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
	skill_area->OnComponentBeginOverlap.AddDynamic(this, &ASlowSkillObject::OnBoxOverlap);
	skill_area->OnComponentEndOverlap.AddDynamic(this, &ASlowSkillObject::OnOverlapEnd);

	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), destroy_delay, false);
}

void ASlowSkillObject::OnBoxOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult
)
{
	bool monster_valid = IsValid(Cast<AMonsterBase>(OtherActor));

	if (monster_valid)
	{
		Cast<AMonsterBase>(OtherActor)->GetCharacterMovement()->MaxWalkSpeed = slow_power;
	}
}

void ASlowSkillObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bool monster_valid = IsValid(Cast<AMonsterBase>(OtherActor));

	if (monster_valid)
	{
		Cast<AMonsterBase>(OtherActor)->ResetMaxWalkSpeed();
	}
}


