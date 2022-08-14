// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AbilityObjectBase.h"
#include "BarrierSkillObject.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ABarrierSkillObject : public AAbilityObjectBase
{
	GENERATED_BODY()
public:
	ABarrierSkillObject();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBoxOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool isFromSweep,
			const FHitResult& SweepResult
		);

public:
	UPROPERTY(EditAnywhere)
	bool use_niagara = false;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* defense_effect_niagara;
	UPROPERTY(EditAnywhere)
	UParticleSystem* defense_effect_particle;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* skill_area;

	UPROPERTY(EditAnywhere)
	class USoundCue* skill_sound;

	UPROPERTY(EditAnywhere)
	float destroy_delay = 0.5f;
};
