// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Turret.generated.h"

UCLASS()
class SCIFICOMBAT_API ATurret : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Destroyed() override;

public:

	UFUNCTION()
	void Attack();
	UFUNCTION(Server, Reliable)
	void ServerAttack();
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastAttack();
	
	UFUNCTION()
	bool TargetMonsterValid();

	UFUNCTION()
	FORCEINLINE void SetTargetMonster(class APawn* _monster) { target_monster = _monster; }

	UFUNCTION()
	FORCEINLINE void SetTurretAIController(class ATurretAIController* _controller) { turret_ai_controller = _controller; }

	UFUNCTION()
	void OnSeePawn(class APawn* pawn);

	UFUNCTION()
	void StateBoardKeyProcess();

	UFUNCTION()
	void TurretFire();

public:
	UPROPERTY()
	class APawn* target_monster;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* spawn_effect;

	UPROPERTY(EditAnywhere)
	class USoundCue* spawn_sound;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* destroyed_effect;

	UPROPERTY(EditAnywhere)
	class USoundCue* destroyed_sound;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* fire_effect;

	UPROPERTY(EditAnywhere)
	class USoundCue* fire_sound;

	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* sensing_component;

	UPROPERTY()
	class ATurretAIController* turret_ai_controller;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> projectile_class;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	class USceneComponent* fire_muzzle;

	UPROPERTY()
	bool detect_monster = false;

};
