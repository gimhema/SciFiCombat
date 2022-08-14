// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AbilityObjectBase.h"
#include "TeleportGrenadeObject.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ATeleportGrenadeObject : public AAbilityObjectBase
{
	GENERATED_BODY()
public:
	ATeleportGrenadeObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	class USoundCue* skill_sound;

	UPROPERTY(EditAnywhere)
	float teleport_start = 0.5f;

	UPROPERTY(EditAnywhere)
	float destroy_delay = 0.5f;

	UPROPERTY(EditAnywhere)
	FVector teleport_offset;

	UPROPERTY()
	class ACombatCharacter* teleport_character;
};
