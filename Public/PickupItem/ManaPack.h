// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem/PickupItem.h"
#include "ManaPack.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AManaPack : public APickupItem
{
	GENERATED_BODY()
public:
	AManaPack();
	virtual void Destroyed() override;
public:
	UPROPERTY(EditAnywhere)
	float mana_power = 10.f;

	UPROPERTY(EditAnywhere)
	float mana_time = 5.f;

	UPROPERTY(EditAnywhere)
	UParticleSystem* mana_effect;

	UPROPERTY(EditAnywhere)
	USoundCue* mana_sound;

public:

	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult
	) override;
};
