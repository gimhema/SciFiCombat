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

	// ���� ���̷�Ż �޽�
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	USkeletalMeshComponent* weapon_mesh;

	// ��Ʈ �ڽ�, ���̷�Ż �޽ÿ� ���δ�.
	UPROPERTY(VisibleAnyWhere, Category = "Weapon Property")
	class USphereComponent* weapon_area;

	UPROPERTY(Replicated)
	bool weapon_area_enable = false;

	// ���ݿ� �ִϸ��̼� ��Ÿ��
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
	
	// ���Žÿ� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* smash_montage;
	
	// �ڷ���Ʈ�� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* roar_montage;
	
	//UPROPERTY()
	//TArray<AActor*> combat_characters;
	//UPROPERTY()
	//TArray<FVector> tel_locs;

	// �ξƿ� �ִϸ��̼� ��Ÿ��
	//UPROPERTY(EditAnywhere, Category = Combat)
	//class UAnimMontage* roar_montage;
	
	// Ȧ��� ��ƼŬ, ���̾ư��� ����Ʈ
	UPROPERTY(EditAnywhere, Category = MeleeAttackEffectOption)
	class UNiagaraSystem* hold_effect_niagara;

	UPROPERTY(EditAnywhere, Category = MeleeAttackEffectOption)
	class UNiagaraSystem* hold_impulse_effect_niagara;

	UPROPERTY(EditAnywhere)
	class URadialForceComponent* spawn_radial_force_component;
	// ���� ��ƼŬ���� ������ �ִϸ��̼ǿ��� ó����

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
	// �������� 3~4ȸ���� �ֵθ���
	// ��Ʈ�ڽ��� Ȱ��ȭ��
	UFUNCTION()
	void NormalAttack();
	UFUNCTION(Server, Reliable)
	void ServerNormalAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastNormalAttack();

	// �÷��̾���� �̵��ӵ��� ���Ͻ�Ű�� ���� ����
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


	// �ڷ���Ʈ�� ����, �÷��̾���� ������̰� ����
	UFUNCTION()
	void AbsorptionAndSwing();
	UFUNCTION(Server, Reliable)
	void ServerAbsorptionAndSwing(const TArray<AActor*>& _actors, const TArray<FVector>& _locs);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastAbsorptionAndSwing(const TArray<AActor*>& _actors, const TArray<FVector>& _locs);

	// �������� Į�� ���ϰ� �ֵθ��� �÷��̾� ��ü���� ��� �����ϴ� ���� ����
	UFUNCTION()
	void SmashAttack();
	UFUNCTION(Server, Reliable)
	void ServerSmashAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSmashAttack();

	UFUNCTION()
	void SpawnSmashObj();

	// Ȧ��, ��Ÿ��, �����ð� ���� Impulse ����
	UFUNCTION()
	void Hold();
	UFUNCTION(Server, Reliable)
	void ServerHold();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastHold();

	
public:
	// Util
	// MultiCast �ȿ��� ȣ��
	// ��Ʈ�ڽ� Ȱ��ȭ��
	UFUNCTION()
	void ActivateHitBox();
	UFUNCTION()
	void PlayBossAnimMontage();
};
