// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/HitScanTypeWeapon.h"
#include "ScatterTypeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AScatterTypeWeapon : public AHitScanTypeWeapon
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& hit_target) override;

protected:
	// FVector FireEndScatter(const FVector& fire_start, const FVector& hit_target);

public:

	UPROPERTY(EditAnywhere)
	uint32 num_scatter_bullets = 10;
};
