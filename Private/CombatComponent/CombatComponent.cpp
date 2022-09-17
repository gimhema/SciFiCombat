// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent/CombatComponent.h"
// #include "SciFiCombat/Public/Weapon/CombatWeapon.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"
// #include "SciFiCombat/Public/HUD/SciFiCombatPlayerHUD.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	walk_speed = 600.f;
	aim_walk_speed = 450.f;

}


void UCombatComponent::EquipWeapon(ACombatWeapon* weapon)
{
	if (weapon_owner == nullptr || weapon == nullptr) return;
	
	if (equipped_weapon != nullptr && equipped_sub_weapon == nullptr)
	{
		EquipSubWeapon(weapon);
	}
	else
	{
		EquipPrimaryWeapon(weapon);
	}

	weapon_owner->GetCharacterMovement()->bOrientRotationToMovement = false;
	weapon_owner->bUseControllerRotationYaw = true;

	//weapon_owner->SetWeaponImage();
}

void UCombatComponent::EquipPrimaryWeapon(ACombatWeapon* weapon)
{
	if (equipped_weapon)
	{
		equipped_weapon->DropWeapon();
	}
	equipped_weapon = weapon;
	equipped_weapon->SetWeaponStatus(EWeaponStatus::EWS_Equipped);

	// 별도 함수로 처리
	AttachPrimaryWeaponToSocket(equipped_weapon);
	/*const USkeletalMeshSocket* hand_socket = weapon_owner->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (hand_socket)
	{
		hand_socket->AttachActor(equipped_weapon, weapon_owner->GetMesh());
	}*/

	equipped_weapon->SetOwner(weapon_owner);
	equipped_weapon->SetAmmoText();

	// 근접무기일경우 어차피 0으로 초기화된다.
	if (carried_ammo_map.Contains(equipped_weapon->GetWeaponType()))
	{
		carried_ammo = carried_ammo_map[equipped_weapon->GetWeaponType()];
	}

	controller = controller == nullptr ? Cast<ASciFiCombatPlayerController>(weapon_owner->Controller) : controller;
	if (controller)
	{
		controller->SetCarriedWeaponAmmoText(carried_ammo);
	}

	if (equipped_weapon->equip_weapon_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, equipped_weapon->equip_weapon_sound,
			weapon_owner->GetActorLocation());
	}

	// 웨폰이 밀리웨폰인지 확인하는 로직을 추가한다.
	if (equipped_weapon->isWeaponEmpty())
	{
		ReloadWeapon();
	}

}

void UCombatComponent::EquipSubWeapon(ACombatWeapon* weapon)
{
	if (weapon == nullptr) return;

	equipped_sub_weapon = weapon;
	equipped_sub_weapon->SetWeaponStatus(EWeaponStatus::EWS_EquippedSub);

	if (weapon_owner == nullptr || equipped_sub_weapon == nullptr) return;
	const USkeletalMeshSocket* sub_weapon_socket = weapon_owner->GetMesh()->GetSocketByName(FName("SubWeapon"));
	if (sub_weapon_socket)
	{
		// sub_weapon_socket->AttachActor(ActorToAttach, weapon_owner->GetMesh());
		sub_weapon_socket->AttachActor(equipped_sub_weapon, weapon_owner->GetMesh());
	}
	equipped_sub_weapon->SetOwner(weapon_owner);
}

void UCombatComponent::SwitchWeapon()
{
	ACombatWeapon* weapon_temp = equipped_weapon;
	equipped_weapon = equipped_sub_weapon;
	equipped_sub_weapon = weapon_temp;

	equipped_weapon->SetWeaponStatus(EWeaponStatus::EWS_Equipped);

	AttachPrimaryWeaponToSocket(equipped_weapon);
	/*const USkeletalMeshSocket* hand_socket = weapon_owner->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (hand_socket)
	{
		hand_socket->AttachActor(equipped_weapon, weapon_owner->GetMesh());
	}*/

	equipped_weapon->SetAmmoText();

	if (carried_ammo_map.Contains(equipped_weapon->GetWeaponType()))
	{
		carried_ammo = carried_ammo_map[equipped_weapon->GetWeaponType()];
	}
	controller = controller == nullptr ? Cast<ASciFiCombatPlayerController>(weapon_owner->Controller) : controller;
	if (controller)
	{
		controller->SetCarriedWeaponAmmoText(carried_ammo);
	}

	if (equipped_weapon->equip_weapon_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, equipped_weapon->equip_weapon_sound,
			weapon_owner->GetActorLocation());
	}

	equipped_sub_weapon->SetWeaponStatus(EWeaponStatus::EWS_EquippedSub);
	AttachSubWeaponToSocket(equipped_sub_weapon);

	//weapon_owner->SetWeaponImage();
}

bool UCombatComponent::IsCanSwitchWeapon()
{
	return (equipped_weapon != nullptr && equipped_sub_weapon != nullptr);
}

void UCombatComponent::AttachSubWeaponToSocket(AActor* ActorToAttach)
{
	if (weapon_owner == nullptr || equipped_sub_weapon == nullptr) return;
	const USkeletalMeshSocket* sub_weapon_socket = weapon_owner->GetMesh()->GetSocketByName(FName("SubWeapon"));
	if (sub_weapon_socket)
	{
		sub_weapon_socket->AttachActor(ActorToAttach, weapon_owner->GetMesh());
		// sub_weapon_socket->AttachActor(equipped_sub_weapon, weapon_owner->GetMesh());
	}
}

void UCombatComponent::AttachPrimaryWeaponToSocket(AActor* ActorToAttach)
{
	if (weapon_owner == nullptr) return;
	
	// 여기서 weapon_style이 밀리인 경우
	
	if (Cast<ACombatWeapon>(ActorToAttach)->weapon_style == EWeaponStyle::WST_Melee)
	{
		const USkeletalMeshSocket* hand_socket = weapon_owner->GetMesh()->GetSocketByName(FName("MeleeWeaponSocket"));
		if (hand_socket)
		{
			hand_socket->AttachActor(ActorToAttach, weapon_owner->GetMesh());
		}
	}
	else
	{
		const USkeletalMeshSocket* hand_socket = weapon_owner->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (hand_socket)
		{
			hand_socket->AttachActor(ActorToAttach, weapon_owner->GetMesh());
		}
	}

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (weapon_owner)
	{
		weapon_owner->GetCharacterMovement()->MaxWalkSpeed = walk_speed;

		if (weapon_owner->GetFollowCamera())
		{
			default_aim_fov = weapon_owner->GetFollowCamera()->FieldOfView;
			current_aim_fov = default_aim_fov;
		}
		if (weapon_owner->HasAuthority())
		{
			InitializeCarriedAmmoMap();
		}
	}
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (weapon_owner && weapon_owner->IsLocallyControlled())
	{
		FHitResult hit_result;
		TracePlayerAim(hit_result);
		aim_target = hit_result.ImpactPoint;

		SetCrosshairHUD(DeltaTime);
		InterpAimFov(DeltaTime);
	}

}

void UCombatComponent::SetCrosshairHUD(float _delta)
{
	if (weapon_owner == nullptr || weapon_owner->Controller == nullptr) return;

	controller = controller == nullptr ? Cast<ASciFiCombatPlayerController>(weapon_owner->Controller) : controller;

	if (controller)
	{
		HUD = HUD == nullptr ? Cast< ASciFiCombatPlayerHUD>(controller->GetHUD()) : HUD;
		if (HUD)
		{
			if (equipped_weapon)
			{
				hud_pkg.crosshair_texture = equipped_weapon->crosshair_texture2d;
				
			}
			else
			{
				hud_pkg.crosshair_texture = nullptr;
			}

			// 크로스헤어의 다이나믹을 조절합니다.
			// FMath::GetMappedRangeValueClamped()
			
			FVector2D move_speed(0.f, weapon_owner->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D velocity_range(0.f, 1.f);
			FVector vel = weapon_owner->GetVelocity();
			vel.Z = 0.f;

			crosshair_dynamic_factor = FMath::GetMappedRangeValueClamped(move_speed, velocity_range, vel.Size());

			if (weapon_owner->GetCharacterMovement()->IsFalling())
			{
				crosshair_air_dynamic_factor = FMath::FInterpTo(crosshair_air_dynamic_factor, 2.25f, _delta, 2.25f);
			}
			else
			{
				crosshair_air_dynamic_factor = FMath::FInterpTo(crosshair_air_dynamic_factor, 0.f, _delta, 2.25f);
			}

			SetCrosshairTextureSizeOffset(cross_hair_offset);
			hud_pkg.crosshair_dynamic_movement = crosshair_dynamic_factor + crosshair_air_dynamic_factor;
			HUD->SetHUDPack(hud_pkg);

		}
	}
}

void UCombatComponent::InterpAimFov(float _delta)
{
	if (equipped_weapon == nullptr) return;
	if (isAim)
	{
		current_aim_fov = FMath::FInterpTo(current_aim_fov, equipped_weapon->GetAimFOV(), _delta,
			equipped_weapon->GetAimInterpSpeed());
	}
	else
	{
		current_aim_fov = FMath::FInterpTo(current_aim_fov, default_aim_fov, _delta, aim_interp_speed);
	}
	if (weapon_owner && weapon_owner->GetFollowCamera())
	{
		weapon_owner->GetFollowCamera()->SetFieldOfView(current_aim_fov);
	}
}

void UCombatComponent::SetCrosshairTextureSizeOffset(float _offset)
{
	HUD->SetCrosshairTextureSizeOffset(_offset);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, equipped_weapon);
	DOREPLIFETIME(UCombatComponent, equipped_sub_weapon);
	DOREPLIFETIME(UCombatComponent, isAim);
	DOREPLIFETIME_CONDITION(UCombatComponent, carried_ammo, COND_OwnerOnly);
	DOREPLIFETIME(UCombatComponent, combat_state);
	DOREPLIFETIME(UCombatComponent, combat_style);
	DOREPLIFETIME(UCombatComponent, grapple_trace_start);
	DOREPLIFETIME(UCombatComponent, grapple_trace_end);
}

void UCombatComponent::SetAim(bool _isAim)
{
	if (weapon_owner == nullptr || equipped_weapon == nullptr) return;
	isAim = _isAim;
	ServerSetAim(isAim); 
	if (weapon_owner)
	{
		weapon_owner->GetCharacterMovement()->MaxWalkSpeed = isAim ? aim_walk_speed : walk_speed;
	}
	if (weapon_owner->IsLocallyControlled() && equipped_weapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		weapon_owner->ShowSniperHUD(_isAim);

	}
}

void UCombatComponent::ServerSetAim_Implementation(bool _isAim)
{
	isAim = _isAim;
	if (weapon_owner)
	{
		weapon_owner->GetCharacterMovement()->MaxWalkSpeed = isAim ? aim_walk_speed : walk_speed;
	}
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (equipped_weapon && weapon_owner)
	{
		combat_style = equipped_weapon->weapon_style;
		combat_style_notrepl = equipped_weapon->weapon_style;

		equipped_weapon->SetWeaponStatus(EWeaponStatus::EWS_Equipped);

		AttachPrimaryWeaponToSocket(equipped_weapon);
		/*const USkeletalMeshSocket* hand_socket = weapon_owner->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (hand_socket)
		{
			hand_socket->AttachActor(equipped_weapon, weapon_owner->GetMesh());
		}*/

		weapon_owner->GetCharacterMovement()->bOrientRotationToMovement = false;
		weapon_owner->bUseControllerRotationYaw = true;

		if (equipped_weapon->equip_weapon_sound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, equipped_weapon->equip_weapon_sound,
				weapon_owner->GetActorLocation());
		}

		equipped_weapon->SetAmmoText();
	}
}

void UCombatComponent::OnRep_EquippedSubWeapon()
{
	if (equipped_sub_weapon && weapon_owner)
	{
		equipped_sub_weapon->SetWeaponStatus(EWeaponStatus::EWS_EquippedSub);
		AttachSubWeaponToSocket(equipped_sub_weapon);
	}
}


void UCombatComponent::PressFireKey(bool is_pressed)
{
	is_pressed_fire_key = is_pressed;

	if (is_pressed_fire_key)
	{
		Fire();
	}
}

void UCombatComponent::Fire()
{
	if (CanCheckFire())
	{
		//ServerFire(aim_target);
		//if (equipped_weapon) { is_can_fire = false; }
		//FireTimerStart();
		if (equipped_weapon->weapon_style == EWeaponStyle::WST_Ranger)
		{
			ServerFire(aim_target);
			if (equipped_weapon) { is_can_fire = false; }
			FireTimerStart();
		}
		else if (equipped_weapon->weapon_style == EWeaponStyle::WST_Melee)
		{
			ServerMeleeAttack();
		}
	}
}

void UCombatComponent::ReloadWeapon()
{
	if (carried_ammo > 0 && combat_state != ECombatState::ECS_Reloading && equipped_weapon)
	{
		ServerReload();
		HandleReload();
		// is_locally_reload = true;
	}
}

void UCombatComponent::ServerReload_Implementation()
{
	if (weapon_owner == nullptr || equipped_weapon == nullptr) return;

	combat_state = ECombatState::ECS_Reloading;
	if (!weapon_owner->IsLocallyControlled()) { HandleReload(); }
	// HandleReload();
}

void UCombatComponent::HandleReload()
{
	if (weapon_owner)
	{
		weapon_owner->PlayReloadMontage();
	}
}

int32 UCombatComponent::AmountToReload()
{
	if (equipped_weapon == nullptr) return 0;
	int32 room_in_maxammo = equipped_weapon->GetMaxAmmo() - equipped_weapon->GetAmmo();

	if (carried_ammo_map.Contains(equipped_weapon->GetWeaponType()))
	{
		int32 amount_carried = carried_ammo_map[equipped_weapon->GetWeaponType()];
		int32 least = FMath::Min(room_in_maxammo, amount_carried);
		return FMath::Clamp(room_in_maxammo, 0, least);
	}

	return 0;
}

void UCombatComponent::FinishReloading()
{
	if (weapon_owner == nullptr) return;
	// is_locally_reload = false;
	if (weapon_owner->HasAuthority())
	{
		combat_state = ECombatState::ECS_Unoccupied;
		UpdateAmmo();
	}
	if (is_pressed_fire_key)
	{
		Fire();
	}
}

void UCombatComponent::UpdateAmmo()
{
	if (weapon_owner == nullptr || equipped_weapon == nullptr) return;
	int32 reload_amount = AmountToReload();
	if (carried_ammo_map.Contains(equipped_weapon->GetWeaponType()))
	{
		carried_ammo_map[equipped_weapon->GetWeaponType()] -= reload_amount;
		carried_ammo = carried_ammo_map[equipped_weapon->GetWeaponType()];
	}
	controller = controller == nullptr ? Cast<ASciFiCombatPlayerController>(weapon_owner->Controller) : controller;
	if (controller)
	{
		controller->SetCarriedWeaponAmmoText(carried_ammo);
	}
	equipped_weapon->AddAmmo(-reload_amount);
	// equipped_weapon->AddAmmo(reload_amount);
	equipped_weapon->SetAmmoText();
}

bool UCombatComponent::CanCheckFire()
{
	if (equipped_weapon == nullptr) return false;
	if (!equipped_weapon->isWeaponEmpty() && is_can_fire && combat_state == ECombatState::ECS_Reloading) return true;
	// if (is_locally_reload) return false;
	return !equipped_weapon->isWeaponEmpty() && is_can_fire && combat_state == ECombatState::ECS_Unoccupied;
}

void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize& hit_target)
{
	MultiCastFire(hit_target);
}

void UCombatComponent::MultiCastFire_Implementation(const FVector_NetQuantize& hit_target)
{
	if (equipped_weapon == nullptr)
	{
		return;
	}
	if (weapon_owner && combat_state == ECombatState::ECS_Unoccupied)
	{
		weapon_owner->PlayAnimMontageFire();
		equipped_weapon->Fire(hit_target);
	}
}

void UCombatComponent::ServerMeleeAttack_Implementation()
{
	MultiCastServerMeleeAttack();
}

void UCombatComponent::MultiCastServerMeleeAttack_Implementation()
{
	if (equipped_weapon == nullptr){ return; }
	if (weapon_owner)
	{
		equipped_weapon->ComboProcess();
	}
}

void UCombatComponent::ServerSmashAttack_Implementation()
{
	MultiCastServerSmashAttack();
}
void UCombatComponent::MultiCastServerSmashAttack_Implementation()
{
	equipped_weapon->MeleeSmashAttack();
}

void UCombatComponent::TracePlayerAim(FHitResult& hit_result)
{
	FVector2D view_size;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(view_size);
	}

	FVector2D aim_location(view_size.X / 2.f, view_size.Y / 2.f);
	FVector aim_world_position;
	FVector aim_world_direction;
	bool is_screen_to_world = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		aim_location,
		aim_world_position,
		aim_world_direction
	);
	
	if (is_screen_to_world)
	{
		FVector trace_start = aim_world_position;

		if (weapon_owner)
		{
			float distance_to_player = (weapon_owner->GetActorLocation() - trace_start).Size();
			trace_start += aim_world_direction * (distance_to_player + 100.f);
			// DrawDebugSphere(GetWorld(), trace_start, 16.f, 23, FColor::Red, false);
		}

		FVector trace_end = trace_start + aim_world_direction * LINE_TRACE_LENGTH;
		grapple_trace_start = trace_start;
		grapple_trace_end = trace_end;
		GetWorld()->LineTraceSingleByChannel(
			hit_result,
			trace_start,
			trace_end,
			ECollisionChannel::ECC_Visibility
		);
		if (hit_result.GetActor() && hit_result.GetActor()->Implements<UCrosshairInteractInterface>())
		{
			hud_pkg.crosshair_color = FLinearColor::Red;
		}
		else
		{
			hud_pkg.crosshair_color = FLinearColor::White;
		}
	}
}


void UCombatComponent::FireTimerStart()
{
	if (equipped_weapon == nullptr || weapon_owner == nullptr) return;
	weapon_owner->GetWorldTimerManager().SetTimer(
		fire_timer_handle,
		this,
		&UCombatComponent::FireTimerEnd,
		equipped_weapon->fire_delay
	);
}

void UCombatComponent::FireTimerEnd()
{
	if (equipped_weapon == nullptr) return;
	is_can_fire = true;
	if (is_pressed_fire_key && equipped_weapon->auto_fire_option)
	{
		Fire();
	}
	if (equipped_weapon->isWeaponEmpty())
	{
		ReloadWeapon();
	}
}

void UCombatComponent::OnRep_CarriedAmmo()
{
	controller = controller == nullptr ? Cast<ASciFiCombatPlayerController>(weapon_owner->Controller) : controller;
	if (controller)
	{
		controller->SetCarriedWeaponAmmoText(carried_ammo);
	}
}

void UCombatComponent::InitializeCarriedAmmoMap()
{
	carried_ammo_map.Emplace(EWeaponType::EWT_AssaultRifle, start_ammo_assault_rifle);
	carried_ammo_map.Emplace(EWeaponType::EWT_MissileLauncher, start_ammo_missile_launcher);
	carried_ammo_map.Emplace(EWeaponType::EWT_SniperRifle, start_ammo_sinper_rifle);
	carried_ammo_map.Emplace(EWeaponType::EWT_ShotGun, start_ammo_shotgun);
	carried_ammo_map.Emplace(EWeaponType::EWT_GrenadeLauncher, start_ammo_grenade_launcher);
	// carried_ammo_map.Emplace(EWeaponType::EWT_AssaultRifle, 30);
}

void UCombatComponent::OnRep_CombatState()
{
	switch (combat_state)
	{
	case ECombatState::ECS_Reloading:
		HandleReload();
		break;
	case ECombatState::ECS_Unoccupied:
		if (is_pressed_fire_key) { Fire(); }
		break;
	}
}

void UCombatComponent::PickupAmmoItem(EWeaponType weapon_type, int32 num_ammo)
{
	if (carried_ammo_map.Contains(weapon_type))
	{
		carried_ammo_map[weapon_type] = FMath::Clamp(carried_ammo_map[weapon_type] + num_ammo, 0, max_carried_ammo);	
		UpdateAmmo();
	}
	if (equipped_weapon && equipped_weapon->isWeaponEmpty() && equipped_weapon->GetWeaponType() == weapon_type)
	{
		ReloadWeapon();
	}
}


