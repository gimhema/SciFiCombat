// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem/PickupItem.h"
#include "JumpUpper.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AJumpUpper : public APickupItem
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
	UParticleSystem* upper_effect;

	UPROPERTY(EditAnywhere)
	USoundCue* upper_sound;

public:
	UPROPERTY(EditAnywhere)
	float jump_z_vel = 4000.f;

	UPROPERTY(EditAnywhere)
	float jump_up_time = 10.f;


};
