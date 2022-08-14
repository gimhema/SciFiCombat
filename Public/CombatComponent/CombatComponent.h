// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SciFiCombat/Public/HUD/SciFiCombatPlayerHUD.h"
#include "SciFiCombat/Public/Weapon/WeaponType.h"
#include "SciFiCombat/SciFiCombatType/CombatStatge.h"
#include "SciFiCombat/Public/Weapon/CombatWeapon.h"
#include "CombatComponent.generated.h"

#define LINE_TRACE_LENGTH 80000.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCIFICOMBAT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	friend class ACombatCharacter;

	void EquipWeapon(class ACombatWeapon* weapon);
	void SwitchWeapon();
	bool IsCanSwitchWeapon();

	void ReloadWeapon();
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION()
	void EquipPrimaryWeapon(ACombatWeapon* weapon);
	
	UFUNCTION()
	void EquipSubWeapon(ACombatWeapon* weapon);
	UFUNCTION()
	void AttachSubWeaponToSocket(AActor* ActorToAttach);
	UFUNCTION()
	void AttachPrimaryWeaponToSocket(AActor* ActorToAttach);


	void PressFireKey(bool is_pressed);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetAim(bool _isAim);

	UFUNCTION(Server, Reliable)
	void ServerSetAim(bool _isAim);

	UFUNCTION()
	void OnRep_EquippedWeapon();
	UFUNCTION()
	void OnRep_EquippedSubWeapon();



	void Fire();
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& hit_target);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastFire(const FVector_NetQuantize& hit_target);

	UFUNCTION(Server, Reliable)
	void ServerReload();
	void HandleReload();

	int32 AmountToReload();
	
	void TracePlayerAim(FHitResult& hit_result);

	void SetCrosshairHUD(float _delta);
	void SetCrosshairTextureSizeOffset(float _offset);

	// Melee Attack
	UFUNCTION(Server, Reliable)
	void ServerMeleeAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastServerMeleeAttack();

	// Smash Attack
	UFUNCTION(Server, Reliable)
	void ServerSmashAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastServerSmashAttack();

public:
	// Pickup Item Function
	void PickupAmmoItem(EWeaponType weapon_type, int32 num_ammo);

public:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, BlueprintReadWrite)
	ACombatWeapon* equipped_weapon;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, BlueprintReadWrite)
	EWeaponStyle combat_style;

	UPROPERTY(BlueprintReadWrite)
	EWeaponStyle combat_style_notrepl;

private:
	UPROPERTY()
	class ASciFiCombatPlayerController* controller;
	UPROPERTY()
	class ACombatCharacter* weapon_owner;
	UPROPERTY()
	class ASciFiCombatPlayerHUD* HUD;


	UPROPERTY(ReplicatedUsing = OnRep_EquippedSubWeapon)
	ACombatWeapon* equipped_sub_weapon;


	FVector aim_target;
	FHUDPackage hud_pkg;

	UPROPERTY(Replicated)
	bool isAim;

	UPROPERTY(EditAnywhere)
	float walk_speed;

	UPROPERTY(EditAnywhere)
	float aim_walk_speed;

	bool is_pressed_fire_key;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	float cross_hair_offset = 1.f;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	float cross_hair_rotation_default = 0.f;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	float cross_hair_rotation_max = 16.f;

	// bool is_locally_reload = false;
private:
	void UpdateAmmo();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Croshair
	float crosshair_dynamic_factor;
	float crosshair_air_dynamic_factor;

	// Aiming

	float default_aim_fov;
	UPROPERTY(EditAnywhere, Category = Combat)
	float aim_fov = 30.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float aim_interp_speed = 20.f;

	float current_aim_fov;

	void InterpAimFov(float _delta);

	// Auto Fire
	UPROPERTY(EditAnywhere, Category = Combat)
	bool is_can_fire = true;
	
	bool CanCheckFire();
	
	FTimerHandle fire_timer_handle;

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 carried_ammo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	// Ammo Info about Weapon Type
	TMap<EWeaponType, int32> carried_ammo_map;

	UPROPERTY(EditAnywhere)
	int32 max_carried_ammo = 500;
	UPROPERTY(EditAnywhere)
	int32 start_ammo_assault_rifle = 30;
	UPROPERTY(EditAnywhere)
	int32 start_ammo_missile_launcher = 10;
	UPROPERTY(EditAnywhere)
	int32 start_ammo_sinper_rifle = 30;
	UPROPERTY(EditAnywhere)
	int32 start_ammo_shotgun = 10;
	UPROPERTY(EditAnywhere)
	int32 start_ammo_grenade_launcher = 10;



	void InitializeCarriedAmmoMap();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState combat_state = ECombatState::ECS_Unoccupied;
	
	UFUNCTION()
	void OnRep_CombatState();
public:
	void FireTimerStart();
	void FireTimerEnd();
public:
	// Grapple Tarce Target
	UPROPERTY(Replicated)
	FVector grapple_trace_start;
	UPROPERTY(Replicated)
	FVector grapple_trace_end;
};
