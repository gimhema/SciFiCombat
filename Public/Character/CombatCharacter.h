// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SciFiCombat/SciFiCombatType/StandingTurning.h"
#include "SciFiCombat/Public/Interfaces/CrosshairInteractInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "SciFiCombat/SciFiCombatType/CombatStatge.h"
//#include "SciFiCombat/SciFiCombatType/SciFiCombatAbilityType.h"
#include "Components/TimeLineComponent.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"
#include "CombatCharacter.generated.h"

UCLASS()
class SCIFICOMBAT_API ACombatCharacter : public ACharacter, public ICrosshairInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void PressEquip();
	void PressCrouch();
	void PressAim();
	void ReleaseAim();
	void AimOffset(float _delta);
	void PressFire();
	void ReleaseFire();
	void PressReload();
	void PressGrapple();
	void ReleaseGrapple();
	void PressViewSubSkillMenu();
	void PressViewOnCursor();
	virtual void PressAbility1();
	virtual void PressAbility2();
	virtual void PressAbility3();
	virtual void PressAbility4();
	virtual void Jump() override;
	UFUNCTION()
	void ReceiveDamage(AActor* damaged_actor, float damage, 
		const UDamageType* damage_type, class AController* instigator_controller,
		AActor* damage_causer);
	
	// 
	void PollInit();
	
	// Test
	void CharacterSpin(float _delta);

public:
	UPROPERTY(Replicated)
	bool gameplay_disable_option = false;

	UPROPERTY()
	bool input_lock = false;
	FORCEINLINE bool GetInputLock() { return input_lock; }
	FORCEINLINE void SetInputLock(bool lock) { input_lock = lock; }

	void UpdateHealthProgress();
	void UpdateSmashPowerProgress();
	void UpdateManaProgress();

	void UseSmashPower(); // smash_power를 0으로 초기화
	void IncreaseSmashPower(float sp); // smash_power를 증가

	void UseMana(float use_mana); // smash_power를 0으로 초기화
	void IncreaseMana(float mana); // smash_power를 증가

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayAnimMontageFire();
	void PlayDeathMontage();
	void PlayReloadMontage();

	void CallEliminate();
	UFUNCTION(NetMulticast, Reliable)
	void Eliminate();

	virtual void Destroyed() override;

	FTimerHandle death_timer;
	UPROPERTY(EditDefaultsOnly)
	float death_delay = 3.f;
	void DeathTimerFinished();
	
	UFUNCTION()
	void SetWeaponImage();
private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* camera_boom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* follow_camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* info_widget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class ACombatWeapon* overlapp_weapon;
	
	
	UPROPERTY(Replicated)
	int32 combat_IFF = 0;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* combat_component;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UGrappleComponent* grapple_component;

	UPROPERTY(EditAnywhere, Category = SkillOption)
	class USceneComponent* ability_object_muzzle;

	UPROPERTY(EditAnywhere, Category = SkillOption)
	class USceneComponent* buff_ability_effect_scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USciFiAbilityComponent* ability_component;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCrowdControlComponent* cc_component;
	UFUNCTION()
	bool GetIsCCControlled();

	// Ability Texture
	UFUNCTION()
	void SetAbilityTexture();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* main_skill_damage_immune_texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* main_skill_barrier_texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* main_skill_teleport_texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* sub_skill_impulse_texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* sub_skill_blackhole_texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* sub_skill_heal_texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* sub_skill_speedbooster_texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* sub_skill_jumpup_texture;

private:


	UFUNCTION()
	void OnRep_OverlappingWeapon(ACombatWeapon* weapon);

	UFUNCTION(Server, Reliable)
	void EquipPressByServer();

	float aim_offset_yaw;
	float aim_offset_pitch;
	FRotator starting_aim_rotation;

	ETurningStanding turing_state;
	void TurningStanding(float _delta);

	UPROPERTY(EditAnywhere, Category = Combat, Replicated)
	class UAnimMontage* fire_montage;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* death_montage1;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* death_montage2;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* death_montage3;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* reload_montage;


	UPROPERTY()
	class ASciFiCombatPlayerState* player_state;

public:
	void SetOverlappingWeapon(ACombatWeapon* weapon);
	bool IsWeaponEquiped();
	bool IsAim();

	FVector GetAimTarget() const;
	void HideCamera();

	FORCEINLINE float GetAimOffsetYaw() const{ return aim_offset_yaw; }
	FORCEINLINE float GetAimOffsetPitch() const { return aim_offset_pitch; }
	FORCEINLINE ETurningStanding GetTurningState() const { return turing_state; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return follow_camera; }
	FORCEINLINE bool IsDeath() const { return is_death; }
	FORCEINLINE bool GetDisableGameOption() const { return gameplay_disable_option; }
	FORCEINLINE float GetCurrentHealth() const { return current_health; }
	FORCEINLINE void SetCurrentHealth(float health_amount) { current_health = health_amount; }

	FORCEINLINE void SetCurrentMana(float mana) { current_mana = mana; }
	FORCEINLINE float GetMaxMana() const { return max_mana; }
	FORCEINLINE float GetCurrentMana() const { return current_mana; }
	
	FORCEINLINE float GetMaxHealth() const { return max_health; }
	FORCEINLINE float GetCurrentSmashPower() const { return current_smash_power; }
	
	FORCEINLINE float GetMaxSmashPower() const { return max_smash_power; }

	FVector GetFollowCameraLocation();
	FVector GetFollowCameraForwardVector();

	FORCEINLINE UCombatComponent* GetCombatComponent() const { return combat_component; }
	FORCEINLINE USciFiAbilityComponent* GetAbilityComponent() const { return ability_component; }
	FORCEINLINE int32 GetCombatIFF() const { return combat_IFF; }
	ECombatState GetCombatState() const;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperHUD(bool is_show);


	void CombatIFFInitialize();

	UFUNCTION()
	void PlayCombatCharacterAnimMontage(class UAnimMontage* montage);
public:
	UPROPERTY(EditAnywhere)
	float max_yaw = 90.f;
	UPROPERTY(EditAnywhere)
	float min_yaw = -90.f;
	UPROPERTY(EditAnywhere)
	float camera_threshold = 200.f;

public:
	// Character Status
	bool is_death = false;

	UPROPERTY(EditAnywhere, Category = "Status")
	float max_health = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Status")
	float current_health = 100.f;
	UFUNCTION()
	void OnRep_Health();

	UPROPERTY(EditAnywhere, Category = "Status")
	float max_smash_power = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_SmashPower, VisibleAnywhere, Category = "Status")
	float current_smash_power = 0.f;
	UFUNCTION()
	void OnRep_SmashPower();

	UPROPERTY(EditAnywhere, Category = "Status")
	float max_mana = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_Mana, VisibleAnywhere, Category = "Status")
	float current_mana = 0.f;
	UFUNCTION()
	void OnRep_Mana();

	ASciFiCombatPlayerController* scifi_combat_player_controller;

	TSubclassOf<class UUserWidget> sub_skill_select_widget_class;
	class UUserWidget* sub_skill_select_widget;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSubSkillMenu();

public:
	// Evade
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Evade, Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* evade_float_curve;

	UPROPERTY()
	FTimeline learp_time_line;
	// time 0.25
	// value 0.9

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Evade, meta = (AllowPrivateAccess = "true"))
	float lerp_timeline_length = 0.58;

	UPROPERTY(EditAnywhere, Category = Evade, Replicated)
	class UAnimMontage* evade_montage;

	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	class UAnimationAsset* evade_anim_sequence;

	UPROPERTY(EditAnywhere, Category = AttackOption)
	class USceneComponent* cc_marker;

	UPROPERTY(EditAnywhere, Category = Evade, Replicated)
	float evade_distance = 1000.0f;
	UPROPERTY(EditAnywhere, Category = Evade, Replicated)
	float evade_force = 300000000.f;
	UPROPERTY(EditAnywhere, Category = Evade)
	float evade_delay = 2.0f;


	UFUNCTION()
	void PressEvadeKey();
	UFUNCTION()
	void CallEvadeFunc();
	UFUNCTION()
	void EvadeUpdate();
	UFUNCTION()
	void EvadeFinished();
	UFUNCTION()
	void Evade();
	UFUNCTION(Server, Reliable)
	void ServerEvade();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastEvade();

public:
	UPROPERTY(BlueprintReadWrite)
	bool is_jumping = false;
	UPROPERTY(BlueprintReadWrite)
	bool is_can_jump = true;
	UPROPERTY(BlueprintReadWrite)
	bool is_can_dobule_jump = false;
	UPROPERTY(BlueprintReadWrite)
	bool is_view_sub_skill_menu = false;
	UPROPERTY(BlueprintReadWrite)
	bool is_show_mouse_cursor = false;
public:
	// Double Jump
	UFUNCTION()
	void CheckJumpState();
	UPROPERTY(EditAnywhere)
	float double_jump_speed = 1200.f;
	
	UFUNCTION()
	void DoubleJump();
	UFUNCTION(Server, Reliable)
	void ServerDoubleJump();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastDoubleJump();

	UFUNCTION()
	void Awakening();
	UFUNCTION(Server, Reliable)
	void ServerAwakening(ECombatCharacterClass selected_class);
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastAwakening(ECombatCharacterClass selected_class);

	UPROPERTY(EditAnywhere)
	float awakening_delay = 3.0f;

	UPROPERTY(BlueprintReadWrite)
	ECombatCharacterClass selected_character_class;
public:
	// Utility
	UFUNCTION()
	void ForceMoveCharacter(float launch_distance, float launch_force);
	UFUNCTION()
	void TeleportCharacter(FVector loc);
	UFUNCTION(Server, Reliable)
	void ServerTeleportCharacter(FVector loc);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastTeleportCharacter(FVector loc);
public:
	UFUNCTION()
	void UseMainAbility();
	UFUNCTION()
	void UseSubAbility();
	UFUNCTION()
	bool ManaCheck(float need_mana); // true일 경우 마나를 깎고 실행, false면 실행이안됨
	UFUNCTION()
	bool SmashPowerCheck(float need_power); // true일 경우 스매시 파워를 깎고 실행, false면 실행이안됨
	UFUNCTION()
	void BeepAbility();
	UPROPERTY(EditAnywhere)
	class USoundCue* beep_sound;
	
	UFUNCTION()
	void SetAbilityAlarmText(FString alram_str);

	UPROPERTY()
	FString available_sub_ability;
	FORCEINLINE void SetAvailableSubAbility(FString ability_name) { available_sub_ability = ability_name; }
	FORCEINLINE FString GetAvailableSubAbility() const { return available_sub_ability; }

	UPROPERTY(Replicated)
	bool damage_immune = false;
	UFUNCTION()
	void SetDamageImmune(bool value);
	UFUNCTION(Server, Reliable)
	void ServerSetDamageImmune(bool value);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSetDamageImmune(bool value);
	FORCEINLINE bool GetDamageImmune() const { return damage_immune; }

public:
	// Inventory
	UFUNCTION(BlueprintImplementableEvent)
	void ReloadInventory();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InventoryInfo, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* inventory_component;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* item_collection_sphere;

	void InteractPickupItem();
// Depercated
public:
	UPROPERTY(BlueprintReadWrite)
	bool anim_control_test_flag = true;

	UPROPERTY(EditAnywhere, Category = Grapple)
	class UCableComponent* grapple_cable_component;

	UPROPERTY(EditAnywhere)
	FVector grapple_local_offset;

	UPROPERTY(EditAnywhere)
	FVector grapple_local_end_offset;

	UPROPERTY(Replicated)
	FVector grapple_point_location;
	UPROPERTY(Replicated)
	FVector grapple_force_vec;
	UPROPERTY(Replicated)
	float current_grapple_force_vec;


	UPROPERTY(Replicated)
	bool is_grappling = false;
	UPROPERTY(Replicated)
	float move_right_temp = 0.f;
	UPROPERTY(EditAnywhere)
	float grapple_min_dst = 100.f;
	UPROPERTY(EditAnywhere)
	int32 grapple_force = 300000000;

	UFUNCTION()
	void CallPressGrappleEvent();
	UFUNCTION(Server, Reliable)
	void ServerFireGrappleHook();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastFireGrappleHook();
	UFUNCTION()
	void FireGrappleHook();
	UFUNCTION()
	void CallReleaseGrappleEvent();
	UFUNCTION(Server, Reliable)
	void ServerReleaseGrappleHook();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastReleaseGrappleHook();

	UFUNCTION()
	void ReleaseGrappleHook();

	UFUNCTION()
	void GrappleSpin();

	void CallAddGrappleForce();
	UFUNCTION(Server, Reliable)
	void ServerAddGrappleForce();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastAddGrappleForce();

	void AddGrappleForce();
};