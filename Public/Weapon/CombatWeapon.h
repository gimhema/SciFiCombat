// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponType.h"
#include "CombatWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponStatus : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_EquippedSub UMETA(DisplayName = "EquippedSub"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponStyle : uint8
{
	WST_Ranger UMETA(DisplayName = "Ranger Weapon"),
	WST_Melee UMETA(DisplayName = "Melee Weapon")
};

UCLASS()
class SCIFICOMBAT_API ACombatWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACombatWeapon();
	void ShowPickUpWeaponWidget(bool isShowWidget);
	virtual void Fire(const FVector& hit_target);
	virtual void ComboProcess();
	void DropWeapon();
	void AddAmmo(int32 added_ammo);
protected:
	virtual void BeginPlay() override;
	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSub();

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

public:
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	USkeletalMeshComponent* weapon_mesh;

	UPROPERTY(VisibleAnyWhere, Category = "Weapon Property")
	class USphereComponent* weapon_area;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponStatus, VisibleAnywhere)
	EWeaponStatus weapon_status;

	UPROPERTY(VisibleAnyWhere, Category = "Weapon Property")
	class UWidgetComponent* pickup_widget;

	UPROPERTY(EditAnywhere, Category = "Waepon Property")
	class UAnimationAsset* fire_animation;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	class UTexture2D* weapon_image;

	UFUNCTION()
	void OnRep_WeaponStatus();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	void SetAmmoText();
	void SetWeaponStatus(EWeaponStatus status);
	FORCEINLINE USphereComponent* GetAreadSphere() const { return weapon_area; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return weapon_mesh; }
	FORCEINLINE float GetAimFOV() const{ return aim_fov; }
	FORCEINLINE float GetAimInterpSpeed() const{ return aim_interp_speed; }
	FORCEINLINE int32 GetAmmo() const { return ammo; }
	FORCEINLINE int32 GetMaxAmmo() const { return max_ammo; }
	bool isWeaponEmpty();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponStyle weapon_style = EWeaponStyle::WST_Ranger;
	FORCEINLINE EWeaponStyle GetWeaponStyle() const { return weapon_style; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType weapon_type;
	FORCEINLINE EWeaponType GetWeaponType() const { return weapon_type; }
public:
	// Cross Hair Info
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	class UTexture2D* crosshair_texture2d;

	UPROPERTY(EditAnywhere)
	float aim_fov = 30.f;
	UPROPERTY(EditAnywhere)
	float aim_interp_speed = 20.f;

public:
	// Auto Fire Property
	UPROPERTY(EditAnywhere, Category = Combat)
	bool auto_fire_option = true;
	UPROPERTY(EditAnywhere, Category = Combat)
	float fire_delay = 0.15f;

	UPROPERTY(EditAnywhere)
	class USoundCue* equip_weapon_sound;
	
public:
	// Ammo
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
	int32 ammo;

	UFUNCTION()
	void OnRep_Ammo();

	void SpendRound();

	UPROPERTY(EditAnywhere)
	int32 max_ammo;

	UPROPERTY()
	class ACombatCharacter* weapon_owner_character;
	UPROPERTY()
	class ASciFiCombatPlayerController* weapon_owner_controller;

public:

	UFUNCTION()
	virtual void MeleeDashSmashAttack();

	UFUNCTION()
	virtual void MeleeSmashAttack();
	// Melee Property
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	int32 melee_combo_count_max = 0;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float melee_combo_reset_time = 0.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float melee_combo_reset_delay = 0.f;
	UPROPERTY(EditAnywhere, Category = MeleeAttackOption)
	float melee_attack_enable_delay = 0.f;

	UPROPERTY(Replicated)
	int32 melee_combo_count = 0;
	UPROPERTY(Replicated)
	bool melee_hit_enable = false;
	UPROPERTY(Replicated)
	bool melee_weapon_enable = false;
	UPROPERTY(Replicated)
	bool is_can_melee_attack = true;


	UFUNCTION()
	void SetWeaponOwnerInputLock(bool lock);
	UFUNCTION()
	bool GetWeaponOwnerInputLock();

public:
	// Outline
	UPROPERTY(EditAnywhere)
	int32 outline_depth_color = 250; // Purple : 250, Blue : 251

	void EnableCustomDepth(bool is_enable);


};







