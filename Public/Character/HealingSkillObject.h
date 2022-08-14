// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AbilityObjectBase.h"
#include "HealingSkillObject.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AHealingSkillObject : public AAbilityObjectBase
{
	GENERATED_BODY()
public:
	AHealingSkillObject();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSphereOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool isFromSweep,
			const FHitResult& SweepResult
		);

public:
	UPROPERTY(EditAnywhere)
	class USphereComponent* skill_area;

	UPROPERTY(EditAnywhere)
	class USoundCue* skill_sound;

	UPROPERTY(EditAnywhere)
	float heal_power = 100.f;

	UPROPERTY(EditAnywhere)
	float heal_time = 5.f;
	
	UPROPERTY(EditAnywhere)
	float destroy_delay = 0.5f;

};
