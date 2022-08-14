// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ProjectileBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "ExplosionProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AExplosionProjectile : public AProjectileBase
{
	GENERATED_BODY()

protected:

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImulse, const FHitResult& Hit);
public:
	AExplosionProjectile();
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* missile_mesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* missile_sk_mesh;
	
public:
	FTimerHandle bomb_timer_handle;
	UPROPERTY(EditAnywhere)
	float bomb_time = 2.0f;
	void BombTimerStart();
	void BombTimerEnd();

};
