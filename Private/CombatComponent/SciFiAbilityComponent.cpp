// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent/SciFiAbilityComponent.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SciFiCombat/Public/Character/AbilityObjectBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "SciFiCombat/Public/Character/Turret.h"

//#include "SciFiCombat/SciFiCombatType/SciFiCombatAbilityType.h"
// Sets default values for this component's properties
USciFiAbilityComponent::USciFiAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USciFiAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitializeAbilityDelegates();
}


// Called every frame
void USciFiAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	HealRampUp(DeltaTime);
}

void USciFiAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USciFiAbilityComponent, combat_master_mesh);
	DOREPLIFETIME(USciFiAbilityComponent, megician_mesh);
	DOREPLIFETIME(USciFiAbilityComponent, guardian_mesh);
	
}


void USciFiAbilityComponent::HealCharacter(float _heal_power, float _heal_time)
{
	is_heal = true;
	heal_rate = _heal_power / _heal_time;
	amount_to_heal += _heal_power;
}

void USciFiAbilityComponent::HealRampUp(float DeltaTime)
{
	if (!is_heal || combat_character == nullptr || combat_character->IsDeath())
	{
		return;
	}
	const float heal_frame = heal_rate * DeltaTime;
	combat_character->SetCurrentHealth(FMath::Clamp(combat_character->GetCurrentHealth() + heal_frame, 0.f, combat_character->GetMaxHealth()));
	combat_character->UpdateHealthProgress();
	amount_to_heal -= heal_frame;

	if (amount_to_heal <= 0.f || combat_character->GetCurrentHealth() >= combat_character->GetMaxHealth())
	{
		is_heal = false;
		amount_to_heal = 0.f;
	}
}

void USciFiAbilityComponent::BoostSpeed(float boost_run, float boost_crouch, float buff_time)
{
	if (combat_character == nullptr) return;

	combat_character->GetWorldTimerManager().SetTimer(
		speed_boost_timer,
		this,
		&USciFiAbilityComponent::ResetBooster,
		buff_time
	);

	if (combat_character->GetCharacterMovement())
	{
		combat_character->GetCharacterMovement()->MaxWalkSpeed = boost_run;
		combat_character->GetCharacterMovement()->MaxWalkSpeedCrouched = boost_crouch;
	}
	MulticastSpeedBoost(boost_run, boost_crouch);

}

void USciFiAbilityComponent::ResetBooster()
{
	if (combat_character == nullptr) return;

	if (combat_character->GetCharacterMovement())
	{
		combat_character->GetCharacterMovement()->MaxWalkSpeed = init_run_speed;
		combat_character->GetCharacterMovement()->MaxWalkSpeedCrouched = init_crouch_speed;
		MulticastSpeedBoost(init_run_speed, init_crouch_speed);
	}
}

void USciFiAbilityComponent::MulticastSpeedBoost_Implementation(float boost_run, float boost_crouch)
{
	if (booster_effect_niagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), booster_effect_niagara, combat_character->GetActorLocation(),
			combat_character->GetActorRotation());
	}
	if (combat_character && combat_character->GetCharacterMovement())
	{
		combat_character->GetCharacterMovement()->MaxWalkSpeed = boost_run;
		combat_character->GetCharacterMovement()->MaxWalkSpeedCrouched = boost_crouch;
	}
}

void USciFiAbilityComponent::SetInitSpeed(float run_speed, float crouch_speed)
{
	init_run_speed = run_speed;
	init_crouch_speed = crouch_speed;
}

void USciFiAbilityComponent::JumpUp(float z_vel, float buf_time)
{
	if (combat_character == nullptr) return;

	combat_character->GetWorldTimerManager().SetTimer(
		jump_up_timer,
		this,
		&USciFiAbilityComponent::ResetJumpUp,
		buf_time
	);

	if (combat_character->GetCharacterMovement())
	{
		combat_character->GetCharacterMovement()->JumpZVelocity = z_vel;
	}
	MulticastJumpUp(z_vel);

}

void USciFiAbilityComponent::ResetJumpUp()
{
	if (combat_character == nullptr) return;

	if (combat_character->GetCharacterMovement())
	{
		combat_character->GetCharacterMovement()->JumpZVelocity = init_jump_z_vel;
		MulticastJumpUp(init_jump_z_vel);
	}
}

void USciFiAbilityComponent::SetInitJumpZ(float z_vel)
{
	init_jump_z_vel = z_vel;
}

void USciFiAbilityComponent::MulticastJumpUp_Implementation(float z_vel)
{
	if (jumpup_effect_niagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpup_effect_niagara, combat_character->GetActorLocation(),
			combat_character->GetActorRotation());
	}
	if (combat_character && combat_character->GetCharacterMovement())
	{
		combat_character->GetCharacterMovement()->JumpZVelocity = z_vel;
	}
}


void USciFiAbilityComponent::InitializeAbilityDelegates()
{
	ability_delegates_map.Add("Impulse", CreateAbilityDelegate("Impulse"));
	ability_delegates_map.Add("BlackHole", CreateAbilityDelegate("BlackHole"));
	ability_delegates_map.Add("IncreaseJumpUp", CreateAbilityDelegate("IncreaseJumpUp"));
	ability_delegates_map.Add("SpeedBooster", CreateAbilityDelegate("SpeedBooster"));
	ability_delegates_map.Add("SpawnHealObject", CreateAbilityDelegate("SpawnHealObject"));
}

FAbilityDelagate USciFiAbilityComponent::CreateAbilityDelegate(FString skill_name)
{
	FAbilityDelagate skill_delegate;
	skill_delegate.BindUFunction(this, FName(skill_name));
	return skill_delegate;
}

void USciFiAbilityComponent::ExecuteMainAbility(ECombatCharacterClass selected_character_class)
{
	ServerExecuteMainAbility(selected_character_class);
}

void USciFiAbilityComponent::ServerExecuteMainAbility_Implementation(ECombatCharacterClass selected_character_class)
{
	MulticastExecuteMainAbility(selected_character_class);
}

void USciFiAbilityComponent::MulticastExecuteMainAbility_Implementation(ECombatCharacterClass selected_character_class)
{
	switch (selected_character_class)
	{
	case ECombatCharacterClass::ECC_CombatMaster:
		SpawnTurret();
		//Teleport();
		break;
	case ECombatCharacterClass::ECC_Magician:
		SpawnWall();
		break;
	case ECombatCharacterClass::ECC_Guardian:
		DamageImmune();
		break;
	}
}


void USciFiAbilityComponent::ExecuteSubAbility(const FString& selected_sub_skill_name)
{
	ServerExecuteSubAbility(selected_sub_skill_name);
}

void USciFiAbilityComponent::ServerExecuteSubAbility_Implementation(const FString& selected_sub_skill_name)
{
	MulticastExecuteSubAbility(selected_sub_skill_name);
}

void USciFiAbilityComponent::MulticastExecuteSubAbility_Implementation(const FString& selected_sub_skill_name)
{
	if (ability_delegates_map[selected_sub_skill_name].IsBound()) 
	{ 
		ability_delegates_map[selected_sub_skill_name].Execute();
	}
}

void USciFiAbilityComponent::DamageImmune()
{
	// 파티클 실행
	// 데미지 이뮨 옵션 설정
	combat_character->SetDamageImmune(true);

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// 캐릭터 데미지 이뮨 옵션 리셋
			combat_character->SetDamageImmune(false);
			// 파티클 실행
			
		}), damage_immune_delay, false);
}
void USciFiAbilityComponent::SpawnWall()
{
	if (combat_character == nullptr) return;
	FVector spawn_location = combat_character->ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rotation = combat_character->ability_object_muzzle->GetComponentRotation();
	if (!barrier_object) return;
	if (barrier_object)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = combat_character;
		spawn_params.Instigator = combat_character;
		UWorld* world = GetWorld();

		if (world)
		{
			world->SpawnActor<AAbilityObjectBase>(
				barrier_object,
				spawn_location,
				spawn_rotation,
				spawn_params
				);
		}
	}
}
void USciFiAbilityComponent::Teleport()
{
	// 텔레포트 오브젝트를 스폰한다
	if (combat_character == nullptr) return;
	FVector spawn_location = combat_character->ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rotation = combat_character->ability_object_muzzle->GetComponentRotation();
	if (!teleport_object) return;
	if (teleport_object)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = combat_character;
		spawn_params.Instigator = combat_character;
		UWorld* world = GetWorld();

		if (world)
		{
			AAbilityObjectBase* _spawned_obj = world->SpawnActor<AAbilityObjectBase>(
				teleport_object,
				spawn_location,
				spawn_rotation,
				spawn_params
				);

			FTimerHandle delay_handle;
			FVector teleport_loc;
			GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
				{
					// 딜레이걸고
					// 텔레포트 오브젝트의 위치정보를 지역변수로 가진 후
					teleport_loc = _spawned_obj->GetActorLocation();
					// SetActorLocation(텔레포트 위치)

					if (use_teleport_niagara)
					{
						if (teleport_effect_niagara)
						{
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(
								GetWorld(),
								teleport_effect_niagara,
								combat_character->GetActorLocation(),
								combat_character->GetActorRotation());
						}
					}
					else
					{
						if (teleport_effect_particle)
						{
							UGameplayStatics::SpawnEmitterAtLocation(
								this,
								teleport_effect_particle,
								combat_character->GetActorLocation(),
								combat_character->GetActorRotation()
							);
						}
					}


					combat_character->SetActorLocation(teleport_loc);



					_spawned_obj->Destroy();
				}), teleport_delay, false);

		}
	}
}

void USciFiAbilityComponent::SpawnTurret()
{
	if (combat_character == nullptr) return;
	FVector spawn_location = combat_character->ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rotation = combat_character->ability_object_muzzle->GetComponentRotation();
	if (!turret_class) return;
	if (turret_class)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = combat_character;
		spawn_params.Instigator = combat_character;
		UWorld* world = GetWorld();

		if (world)
		{
			world->SpawnActor<ATurret>(
				turret_class,
				spawn_location,
				spawn_rotation,
				spawn_params
				);
		}
	}
}

void USciFiAbilityComponent::Impulse()
{
	if (combat_character == nullptr) return;
	FVector spawn_location = combat_character->ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rotation = combat_character->ability_object_muzzle->GetComponentRotation();
	if (!impulse_object) return;
	if (impulse_object)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = combat_character;
		spawn_params.Instigator = combat_character;
		UWorld* world = GetWorld();

		if (world)
		{
			world->SpawnActor<AAbilityObjectBase>(
				impulse_object,
				spawn_location,
				spawn_rotation,
				spawn_params
				);
		}
	}
}
void USciFiAbilityComponent::BlackHole()
{
	if (combat_character == nullptr) return;
	FVector spawn_location = combat_character->ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rotation = combat_character->ability_object_muzzle->GetComponentRotation();
	if (!blackhole_object) return;
	if (blackhole_object)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = combat_character;
		spawn_params.Instigator = combat_character;
		UWorld* world = GetWorld();

		if (world)
		{
			world->SpawnActor<AAbilityObjectBase>(
				blackhole_object,
				spawn_location,
				spawn_rotation,
				spawn_params
				);
		}
	}
}
void USciFiAbilityComponent::IncreaseJumpUp()
{
	if (jumpup_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			jumpup_sound,
			combat_character->GetActorLocation()
		);
	}
	JumpUp(jump_z_vel, jump_up_time);
}


void USciFiAbilityComponent::SpeedBooster()
{
	if (speed_booster_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			speed_booster_sound,
			combat_character->GetActorLocation()
		);
	}
	BoostSpeed(booster_run_speed, booster_crouch_speed, booster_time);
}


void USciFiAbilityComponent::SpawnHealObject()
{
	if (combat_character == nullptr) return;
	FVector spawn_location = combat_character->GetActorLocation();
	FRotator spawn_rotation = combat_character->GetActorRotation();
	if (!heal_object) return;
	if (heal_object)
	{
		FActorSpawnParameters spawn_params;
		spawn_params.Owner = combat_character;
		spawn_params.Instigator = combat_character;
		UWorld* world = GetWorld();

		if (world)
		{
			world->SpawnActor<AAbilityObjectBase>(
				heal_object,
				spawn_location,
				spawn_rotation,
				spawn_params
				);
		}
	}
}
