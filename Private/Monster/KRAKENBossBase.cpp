// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/KRAKENBossBase.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SciFiCombat/Public/Monster/AttackObj/MonsterAttackObjBase.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"
#include "SciFiCombat/Public/Weapon/ProjectileBase.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Character/CombatCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

AKRAKENBossBase::AKRAKENBossBase()
{
	SetRootComponent(GetMesh());

	normal_attack_hitbox_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalAttackHitBox1"));
	normal_attack_hitbox_1->SetupAttachment(GetMesh(), FName(TEXT("kraken_arm_socket_1")));
	
	normal_attack_hitbox_2 = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalAttackHitBox2"));
	normal_attack_hitbox_2->SetupAttachment(GetMesh(), FName(TEXT("kraken_arm_socket_2")));
	
	normal_attack_hitbox_3 = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalAttackHitBox3"));
	normal_attack_hitbox_3->SetupAttachment(GetMesh(), FName(TEXT("kraken_arm_socket_3")));
	
	normal_attack_hitbox_4 = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalAttackHitBox4"));
	normal_attack_hitbox_4->SetupAttachment(GetMesh(), FName(TEXT("kraken_arm_socket_4")));
	
	normal_attack_hitbox_5 = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalAttackHitBox5"));
	normal_attack_hitbox_5->SetupAttachment(GetMesh(), FName(TEXT("kraken_arm_socket_5")));
	
	normal_attack_hitbox_6 = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalAttackHitBox6"));
	normal_attack_hitbox_6->SetupAttachment(GetMesh(), FName(TEXT("kraken_arm_socket_6")));
	
	normal_attack_hitbox_7 = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalAttackHitBox7"));
	normal_attack_hitbox_7->SetupAttachment(GetMesh(), FName(TEXT("kraken_arm_socket_7")));

	normal_attack_hitbox_8 = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalAttackHitBox8"));
	normal_attack_hitbox_8->SetupAttachment(GetMesh(), FName(TEXT("kraken_arm_socket_8")));

	projectile_fire_muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("KrakenFireMuzzle"));
	projectile_fire_muzzle->SetupAttachment(RootComponent);

	kraken_radial_force_component = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	kraken_radial_force_component->SetupAttachment(RootComponent);

	normal_attack_hitbox_1->OnComponentBeginOverlap.AddDynamic(this, &AKRAKENBossBase::KRAKENNormalAttackHit);
	normal_attack_hitbox_2->OnComponentBeginOverlap.AddDynamic(this, &AKRAKENBossBase::KRAKENNormalAttackHit);
	normal_attack_hitbox_3->OnComponentBeginOverlap.AddDynamic(this, &AKRAKENBossBase::KRAKENNormalAttackHit);
	normal_attack_hitbox_4->OnComponentBeginOverlap.AddDynamic(this, &AKRAKENBossBase::KRAKENNormalAttackHit);
	normal_attack_hitbox_5->OnComponentBeginOverlap.AddDynamic(this, &AKRAKENBossBase::KRAKENNormalAttackHit);
	normal_attack_hitbox_6->OnComponentBeginOverlap.AddDynamic(this, &AKRAKENBossBase::KRAKENNormalAttackHit);
	normal_attack_hitbox_7->OnComponentBeginOverlap.AddDynamic(this, &AKRAKENBossBase::KRAKENNormalAttackHit);
	normal_attack_hitbox_8->OnComponentBeginOverlap.AddDynamic(this, &AKRAKENBossBase::KRAKENNormalAttackHit);

}

void AKRAKENBossBase::BeginPlay()
{
	Super::BeginPlay();


}


void AKRAKENBossBase::KRAKENNormalAttackHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Kraken Hitbox Overlapped"));
	//ACombatCharacter* damaged_player = Cast<ACombatCharacter>(OtherActor);
	////bool monster_valid = IsValid(Cast<AMonsterBase>(OtherActor));
	//bool monster_valid = IsValid(damaged_player);
	//if (monster_valid && hitbox_enable == true)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Boss Weapon Attack Overlapped"));
	//	UGameplayStatics::ApplyDamage(damaged_player, attack_power, GetController(), this, UDamageType::StaticClass());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Boss Weapon Attack Not Enable"));
	//}
}

//_Implementation

void AKRAKENBossBase::KRAKENNormalAttack()
{
	ServerKRAKENNormalAttack();
}

void AKRAKENBossBase::ServerKRAKENNormalAttack_Implementation()
{
	MultiCastKRAKENNormalAttack();
}

void AKRAKENBossBase::MultiCastKRAKENNormalAttack_Implementation()
{
	GetMesh()->PlayAnimation(normal_attack_anim_sequence, false);

	FTimerHandle spawn_handle;
	GetWorld()->GetTimerManager().SetTimer(spawn_handle, FTimerDelegate::CreateLambda([&]()
		{
			// 오브젝트 스폰
			UWorld* world = GetWorld();
			FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);

			if (world)
			{
				world->SpawnActor<AMonsterAttackObjBase>(
					kraken_normal_attack_obj,
					GetActorLocation(),
					spawn_rot
					);
			}
		}), smash_attack_obj_spawn_time, false);

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}), normal_attack_anim_delay, false);
}

void AKRAKENBossBase::KRAKENSmashAttack()
{
	ServerKRAKENSmashAttack();
}

void AKRAKENBossBase::ServerKRAKENSmashAttack_Implementation()
{
	MultiCastKRAKENSmashAttack();
}

void AKRAKENBossBase::MultiCastKRAKENSmashAttack_Implementation()
{
	GetMesh()->PlayAnimation(smash_anim_sequence, false);

	FTimerHandle spawn_handle;
	GetWorld()->GetTimerManager().SetTimer(spawn_handle, FTimerDelegate::CreateLambda([&]()
		{
			// 오브젝트 스폰
			UWorld* world = GetWorld();
			FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);

			if (world)
			{
				world->SpawnActor<AMonsterAttackObjBase>(
					kraken_smash_obj,
					GetActorLocation(),
					spawn_rot
					);
			}
		}), smash_attack_obj_spawn_time, false);

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}), smash_attack_delay, false);
}

void AKRAKENBossBase::KRAKENFireProjectile()
{
	ServerKRAKENFireProjectile();
}

void AKRAKENBossBase::ServerKRAKENFireProjectile_Implementation()
{
	MultiCastKRAKENFireProjectile();
}

void AKRAKENBossBase::MultiCastKRAKENFireProjectile_Implementation()
{
	GetMesh()->PlayAnimation(fire_projectile_anim_sequence, false);

	kraken_radial_force_component->FireImpulse();

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}), fire_proj_delay, false);
}

