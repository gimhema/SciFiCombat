// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ProjectileBase.h"
#include "MonsterProjectileBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AMonsterProjectileBase : public AProjectileBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImulse, const FHitResult& Hit) override;
};
