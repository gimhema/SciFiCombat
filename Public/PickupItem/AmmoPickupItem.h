// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem/PickupItem.h"
#include "SciFiCombat/Public/Weapon/WeaponType.h"
#include "AmmoPickupItem.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AAmmoPickupItem : public APickupItem
{
	GENERATED_BODY()
protected:
		virtual void OnSphereOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool isFromSweep,
			const FHitResult& SweepResult
		);

private:
	UPROPERTY(EditAnywhere)
	int32 num_ammo = 30;

	UPROPERTY(EditAnywhere)
	EWeaponType weapon_type;
};
