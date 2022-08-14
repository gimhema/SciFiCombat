// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Turret.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SciFiCombat/Public/Monster/AttackObj/MonsterAttackObjBase.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SceneComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Monster/MonsterBase.h"
#include "SciFiCombat/Public/Weapon/ProjectileBase.h"
#include "SciFiCombat/Public/Character/TurretAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
// Sets default values
ATurret::ATurret()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	sensing_component = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
	sensing_component->OnSeePawn.AddDynamic(this, &ATurret::OnSeePawn);

	fire_muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("FireMuzzle"));
	fire_muzzle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	if (spawn_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			spawn_effect,
			GetActorTransform()
		);
	}
	if (spawn_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			spawn_sound,
			GetActorLocation()
		);
	}
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StateBoardKeyProcess();
}

// Called to bind functionality to input
void ATurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATurret::Destroyed()
{
	Super::Destroyed();
	if (destroyed_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			destroyed_effect,
			GetActorTransform()
		);
	}
	if (destroyed_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			destroyed_sound,
			GetActorLocation()
		);
	}
}


void ATurret::Attack()
{
	ServerAttack();
}

void ATurret::ServerAttack_Implementation()
{
	NetMulticastAttack();
}

void ATurret::NetMulticastAttack_Implementation()
{
	TurretFire();
}

bool ATurret::TargetMonsterValid()
{
	return detect_monster;
}

void ATurret::OnSeePawn(class APawn* pawn)
{
	if (Cast<ACombatCharacter>(pawn))
	{
		detect_monster = false;
	}
	else
	{
		target_monster = pawn;
		//SetTargetMonster(pawn);
		detect_monster = true;
	}
}

void ATurret::StateBoardKeyProcess()
{
	if (!turret_ai_controller) return;

	if (detect_monster == false)
	{
		// State 0 - Rush to Base
		turret_ai_controller->ChangeTurretState(0);
		/*
		aiController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, base_location);
		*/
	}
	else
	{
		turret_ai_controller->ChangeTurretState(1);
		//if (target_monster)
		//{
		//	// 현재 거리가 공격거리보다 멀거나(크거나) 같은가?
		//	// State 1 - Rush to Player 돌진한다.
		//	turret_ai_controller->ChangeTurretState(1);
		//}
	}
}

void ATurret::TurretFire()
{
	UWorld* world = GetWorld();

	if (fire_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			fire_effect,
			fire_muzzle->GetComponentLocation()
		);
	}
	if (fire_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			fire_sound,
			fire_muzzle->GetComponentLocation()
		);
	}

	if (world)
	{
		APawn* instigator = Cast<APawn>(GetOwner());
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = GetOwner();
		spawn_params.Instigator = instigator;

		world->SpawnActor<AProjectileBase>(
			projectile_class,
			fire_muzzle->GetComponentLocation(),
			fire_muzzle->GetComponentRotation(),
			spawn_params
			);
		//world->SpawnActor<AProjectileBase>(
		//	projectile_class,
		//	fire_muzzle->GetComponentLocation(),
		//	fire_muzzle->GetComponentRotation()
		//	);
	}
}

