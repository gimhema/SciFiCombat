// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CombatWeapon.h"
#include "ProjectileTypeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AProjectileTypeWeapon : public ACombatWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& hit_target) override;


private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> projectile_class;
	
};
