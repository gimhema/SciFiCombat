// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterClassSpawner.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/Character/CombatMasterBase.h"
#include "SciFiCombat/Public/Character/MagicianBase.h"
#include "SciFiCombat/Public/Character/GuardianBase.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACharacterClassSpawner::ACharacterClassSpawner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ACharacterClassSpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacterClassSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterClassSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Anim Add
	DOREPLIFETIME(ACharacterClassSpawner, combat_master);
	DOREPLIFETIME(ACharacterClassSpawner, magician);
	DOREPLIFETIME(ACharacterClassSpawner, guardian);
}

// Called to bind functionality to input
void ACharacterClassSpawner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Summon", IE_Pressed, this, &ACharacterClassSpawner::SummonCharacter);
}

void ACharacterClassSpawner::SummonCharacter()
{
	SpawnSelectCharacter();
}

void ACharacterClassSpawner::SpawnSelectCharacter()
{
	ServerSpawnSelectCharacter();
}

void ACharacterClassSpawner::ServerSpawnSelectCharacter_Implementation()
{

	MultiCastSpawnSelectCharacter();
}

void ACharacterClassSpawner::MultiCastSpawnSelectCharacter_Implementation()
{

}

void ACharacterClassSpawner::SpawnCharacter()
{

}