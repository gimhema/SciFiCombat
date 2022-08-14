// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

UCLASS()
class SCIFICOMBAT_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void ReceiveDamage(AActor* damaged_actor, float damage,
			const UDamageType* damage_type, class AController* instigator_controller,
			AActor* damage_causer);

	UFUNCTION()
	virtual void Attack(); // 원거리몬스터는 투사체발사, 근거리 몬스터는 히트박스 활성화

	UFUNCTION()
	void UpdateHealthProgress();

public:
	UPROPERTY()
	class ACombatCharacter* target_player;

	UFUNCTION()
	bool TargetPlayerValid();

	UFUNCTION()
	FORCEINLINE void SetTargetPlayer(class ACombatCharacter* _player) { target_player = _player; }

	UFUNCTION()
	FORCEINLINE void SetMonsterAIController(class AMonsterAIControllerBase* _controller) { monster_ai_controller = _controller; }

	UFUNCTION()
	void OnSeePawn(class APawn* pawn);

	UFUNCTION()
	void StateBoardKeyProcess();

	UFUNCTION()
	void ResetMaxWalkSpeed();
	UFUNCTION()
	void ZeroMaxWalkSpeed();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Destroyed() override;
public:
	UPROPERTY(EditAnywhere, Category = "Status")
	float attack_power = 0.f;
	UPROPERTY(EditAnywhere, Category = "Status")
	float max_health = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Status")
	float current_health = 100.f;
	UPROPERTY(EditAnywhere, Category = "Status")
	double attack_distance = 500.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Status")
	float current_speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float reset_max_walk_speed = 300.f;

	UFUNCTION()
	void UpdateMonsterVelocity();

	UFUNCTION()
	void OnRep_Health();

	UPROPERTY(EditAnywhere)
	class UParticleSystem* destroyed_effect;

	UPROPERTY(EditAnywhere)
	class USoundCue* destroyed_sound;


	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* sensing_component;

	UPROPERTY()
	class AMonsterAIControllerBase* monster_ai_controller;
};



