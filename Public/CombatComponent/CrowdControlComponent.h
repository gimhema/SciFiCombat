// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrowdControlComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCIFICOMBAT_API UCrowdControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCrowdControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY()
	class ACharacter* component_owner;

	UPROPERTY()
	bool is_crowd_controlled = false;
	FORCEINLINE void SetIsCrowdControlled(bool _val) { is_crowd_controlled = _val; }
	FORCEINLINE bool GetIsCrowdControlled() { return is_crowd_controlled; }

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* stun_effect_niagara;
	UPROPERTY(EditAnywhere)
	UParticleSystem* stun_effect_particle;
	UPROPERTY(EditAnywhere)
	bool use_niagara_stun = false;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* airborne_effect_niagara;
	UPROPERTY(EditAnywhere)
	UParticleSystem* airborne_effect_particle;
	UPROPERTY(EditAnywhere)
	bool use_niagara_airborne = false;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* slow_effect_niagara;
	UPROPERTY(EditAnywhere)
	UParticleSystem* slow_effect_particle;
	UPROPERTY(EditAnywhere)
	bool use_niagara_slow = false;

	UPROPERTY()
	FVector effect_spawn_loc;
	UPROPERTY()
	FRotator effect_spawn_rot;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	class USceneComponent* cc_marker;
	FORCEINLINE void SetIsCCMarker(class USceneComponent* _marker) 
	{
		cc_marker = _marker;
	}

public:
	UFUNCTION()
	void InitializeCCComponent(FVector _effect_spawn_loc);

	UFUNCTION()
	void SpawnCrowdControlEffectNiagara(class UNiagaraSystem* niagara_effect,
		FVector spawn_loc, FRotator spawn_rot);
	UFUNCTION()
	void SpawnCrowdControlEffect(UParticleSystem* effect,
		FVector spawn_loc, FRotator spawn_rot);
	
	UFUNCTION()
	void CallStun(float cc_delay);
	UFUNCTION(Server, Reliable)
	void ServerStun(float cc_delay);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastStun(float cc_delay);
	UFUNCTION()
	void Stun(float cc_delay);

	UFUNCTION()
	void CallAirborne(float cc_delay, float _force);
	UFUNCTION(Server, Reliable)
	void ServerAirborne(float cc_delay, float _force);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastAirborne(float cc_delay, float _force);
	UFUNCTION()
	void Airborne(float cc_delay, float _force);

	UFUNCTION()
	void CallSlow(float cc_delay, float slow_speed);
	UFUNCTION(Server, Reliable)
	void ServerSlow(float cc_delay, float slow_speed);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSlow(float cc_delay, float slow_speed);
	UFUNCTION()
	void Slow(float cc_delay, float slow_speed);
};







