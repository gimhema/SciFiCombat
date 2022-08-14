// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"
#include "SciFiAbilityComponent.generated.h"

DECLARE_DELEGATE(FAbilityDelagate)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCIFICOMBAT_API USciFiAbilityComponent : public UActorComponent
{
	GENERATED_BODY()
		
public:	
	// Sets default values for this component's properties
	USciFiAbilityComponent();
	friend class ACombatCharacter;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void HealRampUp(float DeltaTime);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	// Skill List
	// Heal
	void HealCharacter(float _heal_power, float _heal_time);

	// Speed Booster
	void BoostSpeed(float boost_run, float boost_crouch, float buff_time);
	void SetInitSpeed(float run_speed, float crouch_speed);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpeedBoost(float boost_run, float boost_crouch);

	// Jump Apper
	void JumpUp(float z_vel, float buf_time);
	void SetInitJumpZ(float z_vel);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastJumpUp(float z_vel);

public:
	UPROPERTY()
	class ACombatCharacter* combat_character;

	// Heal Property
	bool is_heal = false;
	float heal_rate = 0;
	float amount_to_heal = 0.f;

	// Speed Booster Property
	FTimerHandle speed_boost_timer;
	void ResetBooster();
	float init_run_speed;
	float init_crouch_speed;

	// Jump Apper Property
	FTimerHandle jump_up_timer;
	void ResetJumpUp();
	float init_jump_z_vel;

public:
	UPROPERTY(Replicated, EditAnywhere, Category = Awakening)
	class USkeletalMesh* combat_master_mesh;
	UPROPERTY(Replicated, EditAnywhere, Category = Awakening)
	class USkeletalMesh* megician_mesh;
	UPROPERTY(Replicated, EditAnywhere, Category = Awakening)
	class USkeletalMesh* guardian_mesh;

public:
	// Abilitys
	TMap<FString, FAbilityDelagate> ability_delegates_map;

	FAbilityDelagate CreateAbilityDelegate(FString skill_name);

	void InitializeAbilityDelegates();
	UFUNCTION()
	void ExecuteMainAbility(ECombatCharacterClass selected_character_class); // 파라미터 필요함
	UFUNCTION(Server, Reliable)
	void ServerExecuteMainAbility(ECombatCharacterClass selected_character_class);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastExecuteMainAbility(ECombatCharacterClass selected_character_class);

	UFUNCTION()
	void ExecuteSubAbility(const FString& selected_sub_skill_name);
	UFUNCTION(Server, Reliable)
	void ServerExecuteSubAbility(const FString& selected_sub_skill_name);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastExecuteSubAbility(const FString& selected_sub_skill_name);

public:
	// Main Skills
	UFUNCTION()
	void DamageImmune();
	UPROPERTY(EditAnywhere)
	float damage_immune_delay = 0.f;
	UPROPERTY(EditAnywhere)
	class UTexture2D* damage_immune_ui_texture;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* damage_immune_effect_niagara;

	UFUNCTION()
	void SpawnWall();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAbilityObjectBase> barrier_object;
	UPROPERTY(EditAnywhere)
	class UTexture2D* barrier_ui_texture;
	
	UFUNCTION()
	void Teleport();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAbilityObjectBase> teleport_object;
	UPROPERTY(EditAnywhere)
	class UTexture2D* teleport_ui_texture;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* teleport_effect_niagara;
	UPROPERTY(EditAnywhere)
	UParticleSystem* teleport_effect_particle;
	UPROPERTY(EditAnywhere)
	bool use_teleport_niagara = false;

	UFUNCTION()
	void SpawnTurret();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATurret> turret_class;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* turret_effect_niagara;
	UPROPERTY(EditAnywhere)
	UParticleSystem* turret_effect_particle;
	UPROPERTY(EditAnywhere)
	bool use_turret_niagara = false;

public:
	// Sub Skills
	UFUNCTION()
	void Impulse();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAbilityObjectBase> impulse_object;

	UFUNCTION()
	void BlackHole();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAbilityObjectBase> blackhole_object;
	UPROPERTY(EditAnywhere)
	float teleport_delay = 0.f;


	UFUNCTION()
	void IncreaseJumpUp();
	UPROPERTY(EditAnywhere)
	class USoundCue* jumpup_sound;
	UPROPERTY(EditAnywhere)
	float jump_z_vel = 0.f;
	UPROPERTY(EditAnywhere)
	float jump_up_time = 0.f;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* jumpup_effect_niagara;

	UFUNCTION()
	void SpeedBooster();
	UPROPERTY(EditAnywhere)
	class USoundCue* speed_booster_sound;
	UPROPERTY(EditAnywhere)
	float booster_run_speed = 0.f;
	UPROPERTY(EditAnywhere)
	float booster_crouch_speed = 0.f;
	UPROPERTY(EditAnywhere)
	float booster_time = 0.f;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* booster_effect_niagara;


	UFUNCTION()
	void SpawnHealObject();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAbilityObjectBase> heal_object;
};










