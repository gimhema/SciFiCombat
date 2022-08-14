// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CombatWeapon.h"
#include "HitScanTypeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AHitScanTypeWeapon : public ACombatWeapon
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& hit_target) override;

public:
	UPROPERTY(EditAnywhere)
	float hit_scan_damage = 20.f;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* hit_impact;
	UPROPERTY(EditAnywhere)
	FVector impact_scale;

	UPROPERTY(EditAnywhere)
	UParticleSystem* line_trace_effect;

public:
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float distance_sphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float sphere_radius = 75.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool is_scatter = false;

	FVector FireEndScatter(const FVector& fire_start, const FVector& hit_target);
	void WeaponTraceHit(const FVector& fire_start, const FVector& hit_target, FHitResult& out_hit);
protected:

};
