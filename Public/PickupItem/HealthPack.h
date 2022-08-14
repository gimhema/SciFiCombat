// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem/PickupItem.h"
#include "HealthPack.generated.h"

/**
 * 
 */


UCLASS()
class SCIFICOMBAT_API AHealthPack : public APickupItem
{
	GENERATED_BODY()
public:
	AHealthPack();
	virtual void Destroyed() override;
public:
	UPROPERTY(EditAnywhere)
	float heal_power = 100.f;

	UPROPERTY(EditAnywhere)
	float heal_time = 5.f;

	UPROPERTY(EditAnywhere)
	UParticleSystem* heal_effect;

	UPROPERTY(EditAnywhere)
	USoundCue* heal_sound;

protected:
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult
	);

};
