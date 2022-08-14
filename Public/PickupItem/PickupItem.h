// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class SCIFICOMBAT_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		virtual void OnSphereOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool isFromSweep,
			const FHitResult& SweepResult
		);

	UPROPERTY(EditAnywhere)
	float base_turn_rate = 45.f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere)
	int32 outline_color = 250;


private:

	UPROPERTY(EditAnywhere)
	class USphereComponent* pick_area;

	UPROPERTY(EditAnywhere)
	class USoundCue* pickup_sound;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* pickup_mesh;

	FTimerHandle bind_overlap_timer;
	float bind_overlap_time = 0.25f;
	void BindOverlapTimerFinished();
};
