// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawner/ItemSpawnPoint.h"
#include "SciFiCombat/Public/PickupItem/PickupItem.h"

// Sets default values
AItemSpawnPoint::AItemSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	SpawnTimerStart((AActor*)nullptr);
}

// Called every frame
void AItemSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemSpawnPoint::SpawnPickItem()
{
	int32 num_pickups = pickup_item_class.Num();
	if (num_pickups > 0)
	{
		int32 selection = FMath::RandRange(0, num_pickups - 1);
		spawned_pick_item = GetWorld()->SpawnActor<APickupItem>(pickup_item_class[selection], GetActorTransform());
	
		if (HasAuthority() && spawned_pick_item)
		{
			spawned_pick_item->OnDestroyed.AddDynamic(this, &AItemSpawnPoint::SpawnTimerStart);
		}
	}

}

void AItemSpawnPoint::SpawnTimerFinished()
{
	if (HasAuthority())
	{
		SpawnPickItem();
	}
}

void AItemSpawnPoint::SpawnTimerStart(AActor* destroyed_actor)
{
	const float spawn_time = FMath::RandRange(spawn_time_min, spawn_time_max);
	GetWorldTimerManager().SetTimer(
		spawn_timer,
		this,
		&AItemSpawnPoint::SpawnTimerFinished,
		spawn_time
	);
}
