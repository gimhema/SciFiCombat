// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AbilityObjectBase.h"
#include "SlowSkillObject.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ASlowSkillObject : public AAbilityObjectBase
{
	GENERATED_BODY()
public:
	ASlowSkillObject();
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
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* skill_area;

	UPROPERTY(EditAnywhere)
	class USoundCue* skill_sound;

	UPROPERTY(EditAnywhere)
	float slow_power = 100.0f;

	UPROPERTY(EditAnywhere)
	float destroy_delay = 10.0f;
};
