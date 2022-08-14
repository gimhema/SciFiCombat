// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/Pawn.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/Monster/AI/MonsterAIControllerBase.h"
#include "Sound/SoundCue.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	sensing_component = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
	sensing_component->OnSeePawn.AddDynamic(this, &AMonsterBase::OnSeePawn);

}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		reset_max_walk_speed = GetCharacterMovement()->MaxWalkSpeed;
		OnTakeAnyDamage.AddDynamic(this, &AMonsterBase::ReceiveDamage);
	}
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMonsterVelocity();
	StateBoardKeyProcess();	
}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterBase, current_health);
}

void AMonsterBase::ReceiveDamage(AActor* damaged_actor, float damage,
	const UDamageType* damage_type, class AController* instigator_controller,
	AActor* damage_causer)
{
	current_health = FMath::Clamp(current_health - damage, 0.f, max_health);
	UpdateHealthProgress();
	if (current_health <= 0.f)
	{
		Destroy();
	}
}

void AMonsterBase::UpdateMonsterVelocity()
{
	FVector velocity = GetVelocity();
	velocity.Z = 0.f;
	current_speed = velocity.Size();
}

void AMonsterBase::ResetMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = reset_max_walk_speed;
}

void AMonsterBase::ZeroMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
}

void AMonsterBase::Attack()
{
	
}

void AMonsterBase::OnSeePawn(class APawn* pawn)
{
	SetTargetPlayer(Cast<ACombatCharacter>(pawn));
}

void AMonsterBase::UpdateHealthProgress()
{
	// UI 업데이트
}

void AMonsterBase::StateBoardKeyProcess()
{
	if (!monster_ai_controller) return;

	if (!TargetPlayerValid())
	{
		// State 0 - Rush to Base
		monster_ai_controller->ChangeEnemyState(0);
		/*
		aiController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, base_location);
		*/
	}
	else
	{
		double dist_to_player = FVector::Distance(
			GetActorLocation(),
			target_player->GetActorLocation()
		);

		if (dist_to_player > attack_distance)
		{
			// 현재 거리가 공격거리보다 멀거나(크거나) 같은가?
			// State 1 - Rush to Player 돌진한다.
			monster_ai_controller->ChangeEnemyState(1);
		}
		else
		{
			// 현재 거리가 공격거리보다 좁은가(작은가)?
			// State 2 - Attack to Player 공격한다
			monster_ai_controller->ChangeEnemyState(2);
		}

	}
}

void AMonsterBase::Destroyed()
{
	// 파괴 이펙트 재생
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
			GetActorLocation(),
			GetActorRotation()
		);
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), destroyed_sound, GetActorLocation());
	}

}

void AMonsterBase::OnRep_Health()
{
	UpdateHealthProgress();
}

bool AMonsterBase::TargetPlayerValid()
{
	return (IsValid(target_player) ? true : false);
}
