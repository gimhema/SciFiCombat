// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmashDamageObjectDefault.generated.h"

UCLASS()
class SCIFICOMBAT_API ASmashDamageObjectDefault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmashDamageObjectDefault();

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

	UPROPERTY(EditAnywhere)
	class UBoxComponent* hit_area;

	UPROPERTY(EditAnywhere)
	class USoundCue* hit_sound;

	UPROPERTY(EditAnywhere)
	float destroy_delay = 0.f;

	UPROPERTY(EditAnywhere)
	float obj_damage = 0.f;
};
