// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "WarriorBossBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AWarriorBossBase : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	AWarriorBossBase();
protected:
	virtual void BeginPlay() override;

public:
	//UFUNCTION(Server, Reliable)
	//UFUNCTION(NetMulticast, Reliable)

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	TArray<class ACombatCharacter*> players_arr;

	// 무기 스켈레탈 메시
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	USkeletalMeshComponent* weapon_mesh;

	// 히트 박스, 스켈레탈 메시에 붙인다.
	UPROPERTY(VisibleAnyWhere, Category = "Weapon Property")
	class USphereComponent* weapon_area;

	UPROPERTY(Replicated)
	bool weapon_area_enable = false;

	// 공격용 애니메이션 몽타주
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* normal_attack_montage1;
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* normal_attack_montage2;
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* normal_attack_montage3;
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* normal_attack_montage4;

	UPROPERTY(EditAnywhere, Category = Combat)
	float normal_attack_step_distance = 200.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float normal_attack_step_force = 200.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float teleport_z = 200.f;

	UPROPERTY()
	float origin_max_walk_speed;

	UPROPERTY(EditAnywhere, Category = Combat)
	float slow_delay = 10.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float slow_speed = 100.f;
	
	// 스매시용 애니메이션 몽타주
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* smash_montage;
	
	// 텔레포트용 애니메이션 몽타주
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* roar_montage;
	
	//UPROPERTY()
	//TArray<AActor*> combat_characters;
	//UPROPERTY()
	//TArray<FVector> tel_locs;

	// 로아용 애니메이션 몽타주
	//UPROPERTY(EditAnywhere, Category = Combat)
	//class UAnimMontage* roar_montage;
	
	// 홀드용 파티클, 나이아가라 이펙트
	UPROPERTY(EditAnywhere, Category = MeleeAttackEffectOption)
	class UNiagaraSystem* hold_effect_niagara;

	UPROPERTY(EditAnywhere, Category = MeleeAttackEffectOption)
	class UNiagaraSystem* hold_impulse_effect_niagara;

	UPROPERTY(EditAnywhere)
	class URadialForceComponent* spawn_radial_force_component;
	// 공격 파티클들은 보스의 애니메이션에서 처리함

	UPROPERTY(EditAnywhere, Category = Combat)
	float hold_delay = 1.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float smash_delay = 1.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float smash_airborne_delay = 2.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float smash_airborne_power = 500.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float phase_change_delay = 15.f;
	//UPROPERTY()
	//class AActor* target_player;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonsterAttackObjBase> roar_attack_obj;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABossSmashObjBase> smash_attack_obj;
public:
	virtual void OverlapWeaponHitArea(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapBossWeaponArea(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult);
	// Boss Skills
	UFUNCTION()
	void RandomDesicion();
	// 전방으로 3~4회정도 휘두르기
	// 히트박스가 활성화됨
	UFUNCTION()
	void NormalAttack();
	UFUNCTION(Server, Reliable)
	void ServerNormalAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastNormalAttack();

	// 플레이어들의 이동속도를 저하시키는 액터 스폰
	UFUNCTION()
	void Roar();
	UFUNCTION(Server, Reliable)
	void ServerRoar();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRoar();

	UFUNCTION()
	void SelectTargetPlayer();
	UFUNCTION(Server, Reliable)
	void ServerSelectTargetPlayer(AActor* _actors);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSelectTargetPlayer(AActor* _actors);


	// 텔레포트로 변경, 플레이어들을 끌어들이고 스윙
	UFUNCTION()
	void AbsorptionAndSwing();
	UFUNCTION(Server, Reliable)
	void ServerAbsorptionAndSwing(const TArray<AActor*>& _actors, const TArray<FVector>& _locs);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastAbsorptionAndSwing(const TArray<AActor*>& _actors, const TArray<FVector>& _locs);

	// 전방으로 칼을 강하게 휘두르고 플레이어 전체에게 에어본 적용하는 액터 스폰
	UFUNCTION()
	void SmashAttack();
	UFUNCTION(Server, Reliable)
	void ServerSmashAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSmashAttack();

	UFUNCTION()
	void SpawnSmashObj();

	// 홀드, 딜타임, 일정시간 이후 Impulse 적용
	UFUNCTION()
	void Hold();
	UFUNCTION(Server, Reliable)
	void ServerHold();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastHold();

	
public:
	// Util
	// MultiCast 안에서 호출
	// 히트박스 활성화용
	UFUNCTION()
	void ActivateHitBox();
	UFUNCTION()
	void PlayBossAnimMontage();
};
