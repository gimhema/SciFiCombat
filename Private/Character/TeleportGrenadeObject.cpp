// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TeleportGrenadeObject.h"
#include "Components/BoxComponent.h"
#include "Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "SciFiCombat/Public/Weapon/MonsterProjectileBase.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

ATeleportGrenadeObject::ATeleportGrenadeObject()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//skill_area = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill Area"));
	//skill_area->SetupAttachment(RootComponent);
}


void ATeleportGrenadeObject::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle teleport_handle;
	GetWorld()->GetTimerManager().SetTimer(teleport_handle, FTimerDelegate::CreateLambda([&]()
		{
			FVector teleport_location = GetActorLocation() + teleport_offset;
			if (teleport_character)
			{
				// 여기서 텔레포트
				teleport_character->TeleportCharacter(teleport_location);
			}
		}), teleport_start, false);

	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), destroy_delay, false);
}

