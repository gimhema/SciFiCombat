// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawner/DefenseObj.h"
#include "SciFiCombat/Public/GameMode/SciFiCombatGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADefenseObj::ADefenseObj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADefenseObj::BeginPlay()
{
	Super::BeginPlay();
	
	ASciFiCombatGameMode* game_mode = Cast<ASciFiCombatGameMode>(UGameplayStatics::GetGameMode(this));
	if (game_mode)
	{
		game_mode->InitializeDefenseLocation(GetActorLocation());
	}
}

// Called every frame
void ADefenseObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

