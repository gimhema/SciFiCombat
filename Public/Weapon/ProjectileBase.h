// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class SCIFICOMBAT_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* hit_area;

public:

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* projectile_movement;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* projectile_trail;

	UPROPERTY()
	class UParticleSystemComponent* trail_component;

	UPROPERTY(EditAnywhere)
	UParticleSystem* hit_particle;

	UPROPERTY(EditAnywhere)
	class USoundCue* hit_sound;

public:
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* trail_system_niagara;

	UPROPERTY()
	class UNiagaraComponent* trail_component_niagara;

	void SpawnTrailNiagaraSystem();


protected:
	UPROPERTY(EditAnywhere)
	float projectile_damage = 20.f;

public:
	UPROPERTY(EditAnywhere)
	FVector trail_scale;
	UPROPERTY(EditAnywhere)
	bool is_bounce = false;

};
