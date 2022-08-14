// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem/PickupItem.h"
#include "SpeedBooster.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ASpeedBooster : public APickupItem
{
	GENERATED_BODY()
public:
	virtual void Destroyed() override;

protected:
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult
	);

public:
	UPROPERTY(EditAnywhere)
	UParticleSystem* booster_effect;

	UPROPERTY(EditAnywhere)
	USoundCue* booster_sound;

public:
	UPROPERTY(EditAnywhere)
	float booster_run_speed = 1600.f;
	
	UPROPERTY(EditAnywhere)
	float booster_crouch_speed = 1600.f;

	UPROPERTY(EditAnywhere)
	float booster_time = 1600.f;
};
