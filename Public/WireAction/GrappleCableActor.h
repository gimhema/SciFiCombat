// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CableActor.h"
#include "GrappleCableActor.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AGrappleCableActor : public ACableActor
{
	GENERATED_BODY()
	
public:
	AGrappleCableActor();
	UPROPERTY(EditAnywhere)
	FVector cable_veloicty;

};
