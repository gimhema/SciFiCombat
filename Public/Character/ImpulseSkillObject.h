// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AbilityObjectBase.h"
#include "ImpulseSkillObject.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AImpulseSkillObject : public AAbilityObjectBase
{
	GENERATED_BODY()
public:
	AImpulseSkillObject();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void BurstImpulse();
	UFUNCTION(Server, Reliable)
	void ServerBurstImpulse();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastBurstImpulse();

public:

	UPROPERTY(EditAnywhere)
	class UBoxComponent* hit_area;

	UPROPERTY(EditAnywhere)
	class USphereComponent* skill_area;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* spawn_effect_niagara;

	UPROPERTY(EditAnywhere)
	UParticleSystem* spawn_effect_partice;

	UPROPERTY(EditAnywhere)
	class URadialForceComponent* spawn_radial_force_component;

	UPROPERTY(EditAnywhere)
	bool is_use_niagara = false;

	UPROPERTY(EditAnywhere)
	class USoundCue* skill_sound;

	UPROPERTY(EditAnywhere)
	float burst_delay = 1.5f;

	UPROPERTY(EditAnywhere)
	float destroy_delay = 1.0f;
};
