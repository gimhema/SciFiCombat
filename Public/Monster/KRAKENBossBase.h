// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "KRAKENBossBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AKRAKENBossBase : public AMonsterBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AKRAKENBossBase();

public:

	// 머즐돌리기는 블루프린트로
	// 
	// 
	//OverlapBossWeaponArea(
	//	UPrimitiveComponent* OverlappedComponent,
	//	AActor* OtherActor,
	//	UPrimitiveComponent* OtherComp,
	//	int32 OtherBodyIndex,
	//	bool isFromSweep,
	//	const FHitResult& SweepResult)

	// kraken_arm_socket_
	UPROPERTY(EditAnywhere)
	class UBoxComponent* normal_attack_hitbox_1;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* normal_attack_hitbox_2;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* normal_attack_hitbox_3;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* normal_attack_hitbox_4;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* normal_attack_hitbox_5;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* normal_attack_hitbox_6;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* normal_attack_hitbox_7;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* normal_attack_hitbox_8;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonsterAttackObjBase> kraken_smash_obj;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonsterAttackObjBase> kraken_normal_attack_obj;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> karaken_projectile;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AttackOption)
	class USceneComponent* projectile_fire_muzzle;

	//kraken_radial_force_component = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	//kraken_radial_force_component->SetupAttachment(RootComponent);


	UPROPERTY(EditAnywhere)
	class URadialForceComponent* kraken_radial_force_component;

	UPROPERTY()
	bool hitbox_enable = false;

	UPROPERTY(EditAnywhere, Category = KrakenAttackOption)
	class UAnimationAsset* normal_attack_anim_sequence;
	UPROPERTY(EditAnywhere, Category = KrakenAttackOption)
	class UAnimationAsset* smash_anim_sequence;
	UPROPERTY(EditAnywhere, Category = KrakenAttackOption)
	class UAnimationAsset* fire_projectile_anim_sequence;
	
	UPROPERTY(EditAnywhere, Category = KrakenAttackOption)
	float normal_attack_spawn_delay = 1.0f;
	UPROPERTY(EditAnywhere, Category = KrakenAttackOption)
	float normal_attack_anim_delay = 2.0f;
	UPROPERTY(EditAnywhere, Category = KrakenAttackOption)
	float smash_attack_obj_spawn_time = 2.0f;
	UPROPERTY(EditAnywhere, Category = KrakenAttackOption)
	float smash_attack_delay = 2.5f;
	UPROPERTY(EditAnywhere, Category = KrakenAttackOption)
	float fire_proj_delay = 2.0f;

public:
	UFUNCTION(BlueprintCallable)
	void KRAKENNormalAttack();
	UFUNCTION(Server, Reliable)
	void ServerKRAKENNormalAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastKRAKENNormalAttack();
	
	UFUNCTION(BlueprintCallable)
	void KRAKENSmashAttack();
	UFUNCTION(Server, Reliable)
	void ServerKRAKENSmashAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastKRAKENSmashAttack();


	UFUNCTION(BlueprintCallable)
	void KRAKENFireProjectile();
	UFUNCTION(Server, Reliable)
	void ServerKRAKENFireProjectile();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastKRAKENFireProjectile();

	

	void KRAKENNormalAttackHit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult);
};
