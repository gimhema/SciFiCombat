// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SummonsBeacon.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/Character/CombatMasterBase.h"
#include "SciFiCombat/Public/Character/MagicianBase.h"
#include "SciFiCombat/Public/Character/GuardianBase.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASummonsBeacon::ASummonsBeacon()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASummonsBeacon::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle wait_handle;
	//SpawnSelectCharacter();
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			SpawnSelectCharacter();
		}), spawn_delay, false);
	
	//FTimerHandle destroy_wait_handle;
	//GetWorld()->GetTimerManager().SetTimer(destroy_wait_handle, FTimerDelegate::CreateLambda([&]()
	//	{
	//		Destroy();
	//	}), destroy_delay, false);

}

void ASummonsBeacon::Destroyed()
{
	Super::Destroyed();
}

// Called every frame
void ASummonsBeacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASummonsBeacon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASummonsBeacon::SpawnSelectCharacter()
{
	if (HasAuthority())
	{
		ServerSpawnSelectCharacter();
	}
	//ServerSpawnSelectCharacter();
}

void ASummonsBeacon::ServerSpawnSelectCharacter_Implementation()
{
	MultiCastSpawnSelectCharacter();
}

void ASummonsBeacon::MultiCastSpawnSelectCharacter_Implementation()
{
	if (!Cast<ASciFiCombatPlayerController>(GetController())) { return; }

	ASciFiCombatPlayerController* player_scifi_controller = Cast<ASciFiCombatPlayerController>(GetController());
	ECombatCharacterClass selected_class = player_scifi_controller->selected_character_class;

	FActorSpawnParameters spawn_params;
	spawn_params.Owner = nullptr;
	spawn_params.Instigator = this;


	switch (selected_class)
	{
	case ECombatCharacterClass::ECC_None:
		{
			ACombatCharacter* spawned_none_character = GetWorld()->SpawnActor<ACombatCharacter>(
				none_class,
				GetActorLocation(),
				GetActorRotation(),
				spawn_params
				);
			FTimerHandle possess_wait_handle;
			//GetWorld()->GetTimerManager().SetTimer(possess_wait_handle, FTimerDelegate::CreateLambda([&]()
			//	{
			//		player_scifi_controller->PossessSelectClass(spawned_none_character);
			//	}), 1.0, false);
			//player_scifi_controller->PossessSelectClass(spawned_none_character);
			break;
		}
	case ECombatCharacterClass::ECC_CombatMaster:
		{
			ACombatMasterBase* spawned_cm_character = GetWorld()->SpawnActor<ACombatMasterBase>(
				combat_master_class,
				GetActorLocation(),
				GetActorRotation(),
				spawn_params
				);
			FTimerHandle possess_wait_handle;
			//GetWorld()->GetTimerManager().SetTimer(possess_wait_handle, FTimerDelegate::CreateLambda([&]()
			//	{
			//		player_scifi_controller->PossessSelectClass(spawned_cm_character);
			//	}), 1.0, false);
			//player_scifi_controller->PossessSelectClass(spawned_cm_character);
			break;
		}
	case ECombatCharacterClass::ECC_Magician:
		{
			AMagicianBase* spawned_mag_character = GetWorld()->SpawnActor<AMagicianBase>(
				magician_class,
				GetActorLocation(),
				GetActorRotation(),
				spawn_params
				);
			FTimerHandle possess_wait_handle;
			//GetWorld()->GetTimerManager().SetTimer(possess_wait_handle, FTimerDelegate::CreateLambda([&]()
			//	{
			//		player_scifi_controller->PossessSelectClass(spawned_mag_character);
			//	}), 1.0, false);
			//player_scifi_controller->PossessSelectClass(spawned_mag_character);
			break;

		}
	case ECombatCharacterClass::ECC_Guardian:
		{
			AGuardianBase* spawned_guar_character = GetWorld()->SpawnActor<AGuardianBase>(
				guardian_class,
				GetActorLocation(),
				GetActorRotation(),
				spawn_params
				);
			FTimerHandle possess_wait_handle;
			//GetWorld()->GetTimerManager().SetTimer(possess_wait_handle, FTimerDelegate::CreateLambda([&]()
			//	{
			//		player_scifi_controller->PossessSelectClass(spawned_guar_character);
			//	}), 1.0, false);
			//player_scifi_controller->PossessSelectClass(spawned_guar_character);
			break;
		}
	}

}
