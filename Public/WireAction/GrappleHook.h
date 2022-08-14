// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleComponent.h"
#include "GrappleHook.generated.h"

UCLASS()
class SCIFICOMBAT_API AGrappleHook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnGrappleHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	virtual void Destroyed() override;

	UGrappleComponent* grapple_component;
	FORCEINLINE UGrappleComponent* GetGrappleComponent() const { return grapple_component; }
	void SetGrappleComponent(UGrappleComponent* comp) { grapple_component = comp; }

public:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* hook_mesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* projectile_movement;

	UPROPERTY(EditAnywhere)
	FVector hook_veloicty;

	UPROPERTY(EditAnywhere)
	float max_distance = 2500.0f;

	UPROPERTY(VisibleAnywhere)
	FVector start_location;
};
