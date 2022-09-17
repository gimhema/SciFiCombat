// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/WarriorBossBase.h"
#include "Components/SphereComponent.h"
#include "Character/CombatCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Animation/AnimationAsset.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SciFiCombat/Public/CombatComponent/CrowdControlComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SciFiCombat/Public/Monster/AttackObj/MonsterAttackObjBase.h"
#include "Monster/AI/WarriorBossAIControllerBase.h"
#include "Monster/AttackObj/BossSmashObjBase.h"

AWarriorBossBase::AWarriorBossBase()
{
	weapon_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	weapon_mesh->SetupAttachment(GetMesh(), FName(TEXT("boss_weapon")));

	spawn_radial_force_component = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	spawn_radial_force_component->SetupAttachment(RootComponent);

	weapon_area = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponArea"));
	weapon_area->SetupAttachment(weapon_mesh);
	
}

void AWarriorBossBase::BeginPlay()
{
	Super::BeginPlay();
	//GetAI
	//InitializeControlledBoss(this);
	origin_max_walk_speed = GetCharacterMovement()->MaxWalkSpeed;
	weapon_area->OnComponentBeginOverlap.AddDynamic(this, &AWarriorBossBase::OverlapBossWeaponArea);
}


void AWarriorBossBase::RandomDesicion()
{
	int64 skill_idx = UKismetMathLibrary::RandomInteger64InRange(0, 2);
	switch (skill_idx)
	{
	case 0:
		NormalAttack();
		break;
	case 1:
		SmashAttack();
		break;
	case 2:
		Hold();
		break;
	}
}

void AWarriorBossBase::NormalAttack()
{
	ServerNormalAttack();
}
void AWarriorBossBase::ServerNormalAttack_Implementation()
{
	MultiCastNormalAttack();
}
void AWarriorBossBase::MultiCastNormalAttack_Implementation()
{
	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	FVector evade_foward_velocity = UKismetMathLibrary::GetForwardVector(GetActorRotation()) * normal_attack_step_distance;
	float delay1 = normal_attack_montage1->GetPlayLength() + 1.0;
	float delay2 = normal_attack_montage2->GetPlayLength() + 1.0;
	float delay3 = normal_attack_montage3->GetPlayLength() + 1.0;
	float delay4 = normal_attack_montage4->GetPlayLength() + 1.0;
	weapon_area_enable = true;

	FTimerHandle delay_handle;
	FTimerHandle delay_handl2;
	FTimerHandle delay_handl3;
	FTimerHandle delay_handl4;
	if (anim_instance && normal_attack_montage1)
	{
		anim_instance->Montage_Play(normal_attack_montage1);
	}
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			weapon_area_enable = false;
		}), delay1, false);
}


void AWarriorBossBase::AbsorptionAndSwing()
{
	// 여기서 먼저 GetActor를 통해서 플레이어 배열을 만든다.
	// GetActor를 통해 플레이어들을 감지
	// 플레이어 배열을 만들고
	TArray<AActor*> combat_characters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACombatCharacter::StaticClass(), combat_characters);

	TArray<FVector> tel_locs;
	// 플레이어들을 자신의 주변으로 이동시킴
	for (int i = 0; i < combat_characters.Num(); i++)
	{
		// 위치 계산
		FVector teleport_location;
		// Z는 그대로 두고
		teleport_location.X = GetActorLocation().X + UKismetMathLibrary::RandomFloatInRange(-100, 100);
		teleport_location.Y = GetActorLocation().Y + UKismetMathLibrary::RandomFloatInRange(-100, 100);
		teleport_location.Z = teleport_z;
		tel_locs.Add(teleport_location);
	}
	ServerAbsorptionAndSwing(combat_characters, tel_locs);
}
void AWarriorBossBase::ServerAbsorptionAndSwing_Implementation(const TArray<AActor*>& _actors, const TArray<FVector>& _locs)
{
	MultiCastAbsorptionAndSwing(_actors, _locs);
}
void AWarriorBossBase::MultiCastAbsorptionAndSwing_Implementation(const TArray<AActor*>& _actors, const TArray<FVector>& _locs)
{
	// phase_change_delay 페이즈 변경

	for (int i = 0; i < _actors.Num(); i++)
	{
		_actors[i]->SetActorLocation(_locs[i]);
	}
}


void AWarriorBossBase::Roar()
{
	ServerRoar();
}
void AWarriorBossBase::ServerRoar_Implementation()
{
	MultiCastRoar();
}
void AWarriorBossBase::MultiCastRoar_Implementation()
{
	//FTimerHandle delay_handle;
	//GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
	//	{
	//		//GetBlackboardComponent()->SetValueAsInt(FName("EnemyState"), state);
	//	}), phase_change_delay, false);

	float roar_montage_delay = roar_montage->GetPlayLength();
	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();

	if (anim_instance && normal_attack_montage4)
	{
		anim_instance->Montage_Play(roar_montage);
	}

	UWorld* world = GetWorld();
	FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			if (world)
			{
				world->SpawnActor<AMonsterAttackObjBase>(
					roar_attack_obj,
					GetActorLocation(),
					spawn_rot
					);
			}
		}), roar_montage_delay, false);



	//for (int i = 0; i < _actors.Num(); i++)
	//{
	//	Cast<ACombatCharacter>(_actors[i])->cc_component->CallSlow(slow_delay, slow_speed);
	//}
}


void AWarriorBossBase::SmashAttack()
{
	ServerSmashAttack();

}
void AWarriorBossBase::ServerSmashAttack_Implementation()
{
	MultiCastSmashAttack();
}
void AWarriorBossBase::MultiCastSmashAttack_Implementation()
{
	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (anim_instance && smash_montage)
	{
		anim_instance->Montage_Play(smash_montage);
	}
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			SpawnSmashObj();
		}), smash_delay, false);
}

void AWarriorBossBase::SpawnSmashObj()
{
	UWorld* world = GetWorld();
	FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);
	if (world)
	{
		if (smash_attack_obj)
		{
			world->SpawnActor<ABossSmashObjBase>(
				smash_attack_obj,
				GetActorLocation(),
				spawn_rot
				);
		}
	}
}

void AWarriorBossBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWarriorBossBase, players_arr);
	DOREPLIFETIME(AWarriorBossBase, weapon_area_enable);
}

void AWarriorBossBase::SelectTargetPlayer()
{
	// 여기서 먼저 GetActor를 통해서 플레이어 배열을 만든다.
	// GetActor를 통해 플레이어들을 감지
	// 플레이어 배열을 만들고
	// 랜덤하게 플레이어를 선택
	//TArray<AActor*> combat_characters;
	//if (GetWorld())
	//{
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACombatCharacter::StaticClass(), combat_characters);
	//}
	//int64 idx = UKismetMathLibrary::RandomInteger64InRange(0, combat_characters.Num() - 1);
	//ServerSelectTargetPlayer(combat_characters[idx]);

	// target_player 이거 불러서 쓰면된다.
}
void AWarriorBossBase::ServerSelectTargetPlayer_Implementation(AActor* _actors)
{
	MultiCastSelectTargetPlayer(_actors);
}
void AWarriorBossBase::MultiCastSelectTargetPlayer_Implementation(AActor* _actors)
{
	target_player = Cast<ACombatCharacter>(_actors);
}

void AWarriorBossBase::Hold()
{
	ServerHold();
}
void AWarriorBossBase::ServerHold_Implementation()
{
	MultiCastHold();
}
void AWarriorBossBase::MultiCastHold_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hold_effect_niagara, GetActorLocation(),
		GetActorRotation());

	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (anim_instance && roar_montage)
	{
		anim_instance->Montage_Play(roar_montage);
	}

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// Write Game Logic . . . .
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hold_impulse_effect_niagara, GetActorLocation(),
				GetActorRotation());
			spawn_radial_force_component->FireImpulse();
			GetCharacterMovement()->MaxWalkSpeed = origin_max_walk_speed;
		}), hold_delay, false);
}


void AWarriorBossBase::ActivateHitBox()
{

}
void AWarriorBossBase::PlayBossAnimMontage()
{

}

void AWarriorBossBase::OverlapBossWeaponArea(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult)
{
	//bool target_player_valid = IsValid(Cast<ACombatCharacter>(OtherActor));
	if (weapon_area_enable == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Weapon Attack Overlapped"));
		UGameplayStatics::ApplyDamage(OtherActor, attack_power, GetController(), this, UDamageType::StaticClass());
		//if (target_player_valid)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Target Player Valid"));
		//	UGameplayStatics::ApplyDamage(OtherActor, attack_power, nullptr, this, UDamageType::StaticClass());
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Target Player Not Valid"));
		//	return;
		//}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Weapon Attack Not Enable"));
	}
}


void AWarriorBossBase::OverlapWeaponHitArea(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult)
{
	bool target_player_valid = IsValid(Cast<ACombatCharacter>(OtherActor));
	if (weapon_area_enable == true)
	{
		if (target_player_valid)
		{
			UE_LOG(LogTemp, Warning, TEXT("Boss Weapon Attack OK"));
			UGameplayStatics::ApplyDamage(OtherActor, attack_power, GetController(), this, UDamageType::StaticClass());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Boss Weapon Attack Enabled But Not Character"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Weapon Attack Not Enable"));
	}
}
