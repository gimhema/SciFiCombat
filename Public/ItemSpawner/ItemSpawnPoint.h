// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnPoint.generated.h"

UCLASS()
class SCIFICOMBAT_API AItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APickupItem>> pickup_item_class;

	UPROPERTY()
	APickupItem* spawned_pick_item;

	void SpawnPickItem();
	void SpawnTimerFinished();

	UFUNCTION()
	void SpawnTimerStart(AActor* destroyed_actor);
private:
	FTimerHandle spawn_timer;

	UPROPERTY(EditAnywhere)
	float spawn_time_min;
	UPROPERTY(EditAnywhere)
	float spawn_time_max;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
