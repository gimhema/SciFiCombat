// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterAttackObjBase.generated.h"

UCLASS()
class SCIFICOMBAT_API AMonsterAttackObjBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterAttackObjBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OverlapMeleeHitArea(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult);
	
	FORCEINLINE void SetMonsterController(class AController* _cont) { monster_controller = _cont; }
public:
	UPROPERTY()
	class AController* monster_controller;

	UPROPERTY(VisibleAnyWhere, Category = MeleeAttackOption)
	class USphereComponent* melee_hit_area;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float destroy_delay = 1.0f;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float obj_dmg = 10.0f;
};
