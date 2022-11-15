// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CombatCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "SciFiCombat/Public/Weapon/CombatWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryAction/ItemActionComponent.h"
#include "Inventory/InteractableBase.h"
#include "SciFiCombat/Public/CombatComponent/CombatComponent.h"
#include "SciFiCombat/Public/CombatComponent/SciFiAbilityComponent.h"
#include "SciFiCombat/Public/CombatComponent/CrowdControlComponent.h"
#include "SciFiCombat/Public/WireAction/GrappleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SciFiCombat/SciFiCombat.h"
#include "SciFiCombat/Public/Character/CombatAnimInstance.h"
#include "SciFiCombat/Public/GameMode/SciFiCombatGameMode.h"
#include "TimerManager.h"
#include "SciFiCombat/Public/PlayerState/SciFiCombatPlayerState.h"
#include "SciFiCombat/Public/Weapon/WeaponType.h"
#include "Kismet/GameplayStatics.h"
#include "SciFiCombat/Public/HUD/UserInfoWidget.h"
#include "CableComponent.h"
#include "Components/SceneComponent.h"
#include "SciFiCombat/Public/HUD/SciFiCombatPlayerHUD.h"
#include "SciFiCombat/Public/HUD/PlayerStatusOverlay.h"
#include "SciFiCombat/Public/HUD/SubSkillSelectWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimationAsset.h"

// Sets default values
ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	// SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	camera_boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	camera_boom->SetupAttachment(GetMesh());
	camera_boom->TargetArmLength = 600.f;
	camera_boom->bUsePawnControlRotation = true;
	
	follow_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	follow_camera->SetupAttachment(camera_boom, USpringArmComponent::SocketName);
	follow_camera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->SetIsReplicated(true);

	info_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InfoWidget"));
	info_widget->SetupAttachment(RootComponent);

	combat_component = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	combat_component->SetIsReplicated(true);

	ability_component = CreateDefaultSubobject<USciFiAbilityComponent>(TEXT("Ability Component"));
	ability_component->SetIsReplicated(true);

	cc_component = CreateDefaultSubobject<UCrowdControlComponent>(TEXT("CC Component"));
	cc_component->SetIsReplicated(true);

	ability_object_muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Ability Object Fire Component"));
	ability_object_muzzle->SetIsReplicated(true);
	ability_object_muzzle->SetupAttachment(RootComponent);

	cc_marker = CreateDefaultSubobject<USceneComponent>(TEXT("CCMarker"));
	cc_marker->SetupAttachment(RootComponent);
	
	buff_ability_effect_scene = CreateDefaultSubobject<USceneComponent>(TEXT("Buff Ability Effect Scene"));
	buff_ability_effect_scene->SetIsReplicated(true);
	buff_ability_effect_scene->SetupAttachment(RootComponent);

	
	item_collection_sphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollectionSphere"));
	item_collection_sphere->SetupAttachment(RootComponent);
	item_collection_sphere->SetSphereRadius(200.f);

	inventory_component = CreateDefaultSubobject<UInventoryComponent>(TEXT("ItemInventoryComponent"));
	inventory_component->SetIsReplicated(true);
	
	item_action_component = CreateDefaultSubobject<UItemActionComponent>(TEXT("ItemActionComponent"));
	item_action_component->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetIsReplicated(true);
	evade_float_curve = nullptr;
	lerp_timeline_length = 0.4f;

	turing_state = ETurningStanding::ETURN_STANDING_IDLE;
}

void ACombatCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACombatCharacter, overlapp_weapon, COND_OwnerOnly);

	// Anim Add
	DOREPLIFETIME(ACombatCharacter, fire_montage);
	DOREPLIFETIME(ACombatCharacter, current_health);
	DOREPLIFETIME(ACombatCharacter, current_smash_power);
	DOREPLIFETIME(ACombatCharacter, current_mana);
	DOREPLIFETIME(ACombatCharacter, gameplay_disable_option);
	DOREPLIFETIME(ACombatCharacter, combat_IFF);
	DOREPLIFETIME(ACombatCharacter, is_grappling);
	DOREPLIFETIME(ACombatCharacter, move_right_temp);
	DOREPLIFETIME(ACombatCharacter, grapple_point_location);
	DOREPLIFETIME(ACombatCharacter, grapple_force_vec);
	DOREPLIFETIME(ACombatCharacter, current_grapple_force_vec);
	DOREPLIFETIME(ACombatCharacter, evade_montage);
	DOREPLIFETIME(ACombatCharacter, damage_immune);
	DOREPLIFETIME(ACombatCharacter, is_sprint);
	DOREPLIFETIME(ACombatCharacter, sprint_speed);
	DOREPLIFETIME(ACombatCharacter, sprint_reset_speed);
	DOREPLIFETIME(ACombatCharacter, is_guard);
}

void ACombatCharacter::CombatIFFInitialize()
{
	 combat_IFF = Cast<ASciFiCombatPlayerController>(GetController())->GetPlayerIFF();

	 FString iff_info = FString::FromInt(combat_IFF);
	 Cast<UUserInfoWidget>(info_widget->GetWidget())->SetInfoText(iff_info);
}

// Called when the game starts or when spawned
void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	sprint_reset_speed = GetCharacterMovement()->MaxWalkSpeed;

	inventory_component->SetInventoryOwer(this);
	item_action_component->InitializeItemActionComponent(this);
	UpdateHealthProgress();
	UpdateSmashPowerProgress();
	UpdateManaProgress();
	cc_component->InitializeCCComponent(cc_marker->GetComponentLocation());
	cc_component->SetIsCCMarker(cc_marker);
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ACombatCharacter::ReceiveDamage);
		// grapple_component->SetGrappleOwner(this);
		// CombatIFFInitialize();
	}
	// Delay
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			Awakening();
		}), awakening_delay, false); //반복도 여기서 추가 변수를 선언해 설정가능
	// Delay

}



void ACombatCharacter::PollInit()
{
	if (player_state == nullptr)
	{
		player_state = GetPlayerState<ASciFiCombatPlayerState>();
		if (player_state)
		{
			player_state->AddToScore(0.f);
			player_state->AddToDeathCount(0);
		}
	}
}

void ACombatCharacter::UpdateHealthProgress()
{
	scifi_combat_player_controller = scifi_combat_player_controller == nullptr ?
		Cast<ASciFiCombatPlayerController>(Controller) : scifi_combat_player_controller;
	if (scifi_combat_player_controller)
	{
		scifi_combat_player_controller->SetHealthProgress(current_health, max_health);
	}
}

// Called every frame
void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckJumpState();
	CharacterSpin(DeltaTime);
	// AimOffset(DeltaTime);
	HideCamera();
	PollInit();

	//CollectAutoPickups();
	//CheckForInteractables();
	//GrappleSpin();
}

void ACombatCharacter::CheckJumpState()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		
		is_can_jump = true;
		is_jumping = false;
	}
}

void ACombatCharacter::GrappleSpin()
{
	if (IsLocallyControlled())
	{
		ServerAddGrappleForce();
	}
}

void ACombatCharacter::CallAddGrappleForce()
{
	//Role == ROLE_AutonomousProxy
	//if (HasAuthority())
	//{
	//	MultiCastAddGrappleForce();
	//	//ServerAddGrappleForce();
	//}
	//else
	//{
	//	ServerAddGrappleForce();
	//	//AddGrappleForce();
	//}
}

void ACombatCharacter::ServerAddGrappleForce_Implementation()
{
	MultiCastAddGrappleForce();
}

void ACombatCharacter::MultiCastAddGrappleForce_Implementation()
{
	//AddGrappleForce();

}

void ACombatCharacter::AddGrappleForce()
{

}

void ACombatCharacter::CharacterSpin(float _delta)
{
	if (gameplay_disable_option)
	{	
		bUseControllerRotationYaw = false;
		return;
	}
	AimOffset(_delta);
}

// Called to bind functionality to input
void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACombatCharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ACombatCharacter::PressEquip);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACombatCharacter::PressCrouch);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ACombatCharacter::PressAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ACombatCharacter::ReleaseAim);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACombatCharacter::OnSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACombatCharacter::OffSprint);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACombatCharacter::PressFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACombatCharacter::ReleaseFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ACombatCharacter::PressReload);
	PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &ACombatCharacter::PressEvadeKey);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACombatCharacter::InteractPickupItem);

	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &ACombatCharacter::PressAbility1);
	PlayerInputComponent->BindAction("Ability2", IE_Pressed, this, &ACombatCharacter::PressAbility2);

	PlayerInputComponent->BindAction("Awakening", IE_Pressed, this, &ACombatCharacter::PressAbility3);
	PlayerInputComponent->BindAction("TestKey2", IE_Pressed, this, &ACombatCharacter::PressAbility4);

	PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &ACombatCharacter::PressGuard);
	PlayerInputComponent->BindAction("Guard", IE_Released, this, &ACombatCharacter::ReleaseGuard);

	PlayerInputComponent->BindAction("ShowMouseCursor", IE_Pressed, this, &ACombatCharacter::PressViewOnCursor);
	PlayerInputComponent->BindAction("ViewSubSkillMenu", IE_Pressed, this, &ACombatCharacter::PressViewSubSkillMenu);


	//PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &ACombatCharacter::PressGrapple);
	//PlayerInputComponent->BindAction("Grapple", IE_Released, this, &ACombatCharacter::ReleaseGrapple);
	// PressAbility1

	PlayerInputComponent->BindAxis("MoveForward", this, &ACombatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACombatCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACombatCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACombatCharacter::LookUp);

}

void ACombatCharacter::MoveForward(float value)
{
	if (GetInputLock()) return;

	if (GetIsCCControlled()) return;

	if (gameplay_disable_option) return;
	if (Controller != nullptr && value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, value);
	}
}
void ACombatCharacter::MoveRight(float value)
{
	if (GetInputLock()) return;

	if (GetIsCCControlled()) return;

	if (gameplay_disable_option) return;
	if (Controller != nullptr && value != 0.f)
	{
		move_right_temp = value;
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, value);
	}
}

void ACombatCharacter::PressViewSubSkillMenu()
{
	ShowSubSkillMenu();
}


void ACombatCharacter::PressViewOnCursor()
{
	// 컨트롤러로 온오프
	if (!is_show_mouse_cursor)
	{
		scifi_combat_player_controller->SetShowMouseCursor(true);
		is_show_mouse_cursor = true;
	}
	else
	{
		scifi_combat_player_controller->SetShowMouseCursor(false);
		is_show_mouse_cursor = false;
	}

}

void ACombatCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}
void ACombatCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ACombatCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (combat_component)
	{
		combat_component->weapon_owner = this;
	}
	if (ability_component)
	{
		ability_component->combat_character = this;
		ability_component->SetInitSpeed(GetCharacterMovement()->MaxWalkSpeed, GetCharacterMovement()->MaxWalkSpeedCrouched);
		ability_component->SetInitJumpZ(GetCharacterMovement()->JumpZVelocity);
	}

}

void ACombatCharacter::TurningStanding(float _delta)
{
	if (aim_offset_yaw > 90.f)
	{
		turing_state = ETurningStanding::ETURN_STANDING_RIGHT;
	}
	else if (aim_offset_yaw < -90.f)
	{
		turing_state = ETurningStanding::ETURN_STANDING_LEFT;
	}
}

void ACombatCharacter::AimOffset(float _delta)
{
	if (combat_component && combat_component->equipped_weapon == nullptr) return;
	FVector vel = GetVelocity();
	vel.Z = 0.f;
	float spd = vel.Size();
	bool isAir = GetCharacterMovement()->IsFalling();

	if (spd == 0.f && !isAir)
	{
		FRotator current_aim_rotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator delta_aim_rotation = UKismetMathLibrary::NormalizedDeltaRotator(current_aim_rotation, starting_aim_rotation);
		aim_offset_yaw = delta_aim_rotation.Yaw;
		bUseControllerRotationYaw = true;
		TurningStanding(_delta);
	}
	if (spd > 0.f || isAir)
	{
		
		starting_aim_rotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		aim_offset_yaw = 0.f;
		bUseControllerRotationYaw = true;
		turing_state = ETurningStanding::ETURN_STANDING_IDLE;
	}

	aim_offset_pitch = GetBaseAimRotation().Pitch;
	if (aim_offset_pitch > 90.f && !IsLocallyControlled())
	{
		FVector2D in_range(270.f, 360.f);
		FVector2D out_range(-90.f, 0.f);
		aim_offset_pitch = FMath::GetMappedRangeValueClamped(in_range, out_range, aim_offset_pitch);
	}
}

void ACombatCharacter::PressFire()
{
	if (GetIsCCControlled()) return;
	if (gameplay_disable_option) return;
	if (combat_component)
	{
		combat_component->PressFireKey(true);
	}
}

void ACombatCharacter::PressGrapple()
{
	if (gameplay_disable_option) return;
	CallPressGrappleEvent();
}

void ACombatCharacter::ReleaseGrapple()
{
	if (gameplay_disable_option) return;
	CallReleaseGrappleEvent();
}

void ACombatCharacter::PressReload()
{
	if (GetIsCCControlled()) return;
	if (gameplay_disable_option) return;
	if (combat_component)
	{
		if (combat_component->equipped_weapon->weapon_style != EWeaponStyle::WST_Melee)
		{
			combat_component->ReloadWeapon();
		}
	}
}


void ACombatCharacter::ReleaseFire()
{
	if (gameplay_disable_option) return;
	if (combat_component)
	{
		combat_component->PressFireKey(false);
	}
}

void ACombatCharacter::PressEquip()
{
	if (gameplay_disable_option) return;
	if (combat_component)
	{
		//if (HasAuthority())
		//{
		//	combat_component->EquipWeapon(overlapp_weapon);
		//}
		//else
		//{
		//	EquipPressByServer();
		//}
		EquipPressByServer();
	}
}

void ACombatCharacter::HideCamera()
{
	if (!IsLocallyControlled()) return;
	if ((follow_camera->GetComponentLocation() - GetActorLocation()).Size() < camera_threshold)
	{
		GetMesh()->SetVisibility(false);
		if (combat_component && combat_component->equipped_weapon && 
			combat_component->equipped_weapon->GetWeaponMesh())
		{
			combat_component->equipped_weapon->GetWeaponMesh()->bOwnerNoSee = true;
		}
	}
	else
	{
		GetMesh()->SetVisibility(true);
		if (combat_component && combat_component->equipped_weapon &&
			combat_component->equipped_weapon->GetWeaponMesh())
		{
			combat_component->equipped_weapon->GetWeaponMesh()->bOwnerNoSee = false;
		}
	}
}

void ACombatCharacter::PressCrouch()
{
	if (GetInputLock()) return;

	if (GetIsCCControlled()) return;

	if (gameplay_disable_option) return;
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ACombatCharacter::Jump()
{
	if (GetInputLock()) return;
	
	if (GetIsCCControlled()) return;

	if (gameplay_disable_option) return;
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		is_jumping = true;
		if (is_can_jump)
		{
			if (is_can_dobule_jump)
			{
				DoubleJump();
			}
			else
			{
				Super::Jump();
				is_can_dobule_jump = true;
			}
		}
	}
}

void ACombatCharacter::DoubleJump()
{
	if (HasAuthority())
	{
		ServerDoubleJump();
	}
	else
	{
		ServerDoubleJump();
	}
}

void ACombatCharacter::ServerDoubleJump_Implementation()
{
	MultiCastDoubleJump();
}

void ACombatCharacter::MultiCastDoubleJump_Implementation()
{
	FVector double_jump_vec(0.f, 0.f, double_jump_speed);
	LaunchCharacter(double_jump_vec, false, false);
	is_can_dobule_jump = false;
	is_can_jump = false;
}

void ACombatCharacter::PressAim()
{
	if (gameplay_disable_option) return;
	if (combat_component)
	{
		if (combat_component->equipped_weapon->weapon_style != EWeaponStyle::WST_Melee)
		{
			combat_component->SetAim(true);
		}
		else if (combat_component->equipped_weapon->weapon_style == EWeaponStyle::WST_Melee)
		{
			// Smash
			//combat_component->equipped_weapon->MeleeSmashAttack();
			if (is_sprint)
			{
				combat_component->ServerDashSmashAttack();
			}
			else
			{
				if (SmashPowerCheck(100))
				{
					combat_component->ServerSmashAttack();
				}
			}
		}
	}
}

void ACombatCharacter::ReleaseAim()
{
	if (gameplay_disable_option) return;
	if (combat_component)
	{
		combat_component->SetAim(false);
	}
}


void ACombatCharacter::EquipPressByServer_Implementation()
{
	if (combat_component)
	{
		if (overlapp_weapon)
		{
			combat_component->EquipWeapon(overlapp_weapon);
		}
		else if (combat_component->IsCanSwitchWeapon())
		{
			combat_component->SwitchWeapon();
		}
	}
}

void ACombatCharacter::SetOverlappingWeapon(ACombatWeapon* weapon)
{
	if (overlapp_weapon)
	{
		overlapp_weapon->ShowPickUpWeaponWidget(false);
	}
	overlapp_weapon = weapon;
	if (IsLocallyControlled())
	{
		if (overlapp_weapon)
		{
			overlapp_weapon->ShowPickUpWeaponWidget(true);
		}
	}
}

void ACombatCharacter::OnRep_OverlappingWeapon(ACombatWeapon* weapon)
{
	if (overlapp_weapon)
	{
		overlapp_weapon->ShowPickUpWeaponWidget(true);
	}
	if (weapon)
	{
		weapon->ShowPickUpWeaponWidget(false);
	}
}

bool ACombatCharacter::IsWeaponEquiped()
{
	return (combat_component && combat_component->equipped_weapon);
}

bool ACombatCharacter::IsAim()
{
	return (combat_component && combat_component->isAim);
}

void ACombatCharacter::PlayAnimMontageFire()
{
	if (combat_component == nullptr || combat_component->equipped_weapon == nullptr) return;

	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (anim_instance && fire_montage)
	{
		anim_instance->Montage_Play(fire_montage);
		// FName section_name;
		// section_name = is_aim ? FName("");
		// anim_instance->Montage_JumpToSection(section_name);
	}
}

void ACombatCharacter::PlayCombatCharacterAnimMontage(class UAnimMontage* montage)
{
	if (combat_component == nullptr || combat_component->equipped_weapon == nullptr) return;

	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (anim_instance && montage)
	{
		anim_instance->Montage_Play(montage);
	}
}

void ACombatCharacter::PlayReloadMontage()
{
	if (combat_component == nullptr || combat_component->equipped_weapon == nullptr) return;

	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (anim_instance && reload_montage)
	{
		anim_instance->Montage_Play(reload_montage);
		FName section_name;
		switch (combat_component->equipped_weapon->GetWeaponType())
		{
		case EWeaponType::EWT_AssaultRifle:
			section_name = FName("Rifle");
			break;
		}
		anim_instance->Montage_JumpToSection(section_name);
	}
}

ECombatState ACombatCharacter::GetCombatState() const
{
	if (combat_component == nullptr) return ECombatState::ECS_MAX;

	return combat_component->combat_state;
}


FVector ACombatCharacter::GetAimTarget() const
{
	if (combat_component == nullptr) return FVector();
	
	return combat_component->aim_target;
}


void ACombatCharacter::OnRep_Health()
{
	UpdateHealthProgress();
}

void ACombatCharacter::OnRep_SmashPower()
{
	UpdateSmashPowerProgress();
}

void ACombatCharacter::UseSmashPower()
{
	current_smash_power = 0.f;
	UpdateSmashPowerProgress();
}

void ACombatCharacter::IncreaseSmashPower(float sp)
{
	float temp = current_smash_power + sp;
	if (temp >= max_smash_power)
	{
		current_smash_power = max_smash_power;
	}
	else
	{
		current_smash_power = temp;
	}
	UpdateSmashPowerProgress();
}

void ACombatCharacter::UpdateSmashPowerProgress()
{
	scifi_combat_player_controller = scifi_combat_player_controller == nullptr ?
		Cast<ASciFiCombatPlayerController>(Controller) : scifi_combat_player_controller;
	if (scifi_combat_player_controller)
	{
		 scifi_combat_player_controller->SetSmashPowerProgress(current_smash_power, max_smash_power);
	}
}

void ACombatCharacter::UpdateManaProgress()
{
	scifi_combat_player_controller = scifi_combat_player_controller == nullptr ?
		Cast<ASciFiCombatPlayerController>(Controller) : scifi_combat_player_controller;
	if (scifi_combat_player_controller)
	{
		scifi_combat_player_controller->SetManaProgress(current_mana, max_mana);
	}
}
void ACombatCharacter::UseMana(float use_mana)
{
	float temp = current_mana - use_mana;
	if (temp <= 0.f)
	{
		current_mana = 0.f;
	}
	else
	{
		current_mana = temp;
	}
	UpdateManaProgress();
}
void ACombatCharacter::IncreaseMana(float mana)
{
	float temp = current_mana + mana;
	if (temp >= max_mana)
	{
		current_mana = max_mana;
	}
	else
	{
		current_mana = temp;
	}
	UpdateManaProgress();
}
void ACombatCharacter::OnRep_Mana()
{
	UpdateManaProgress();
}

void ACombatCharacter::ReceiveDamage(AActor* damaged_actor, float damage,
	const UDamageType* damage_type, class AController* instigator_controller,
	AActor* damage_causer)
{
	if (is_guard) { return; }
	if (GetDamageImmune() == true) { return; }

	current_health = FMath::Clamp(current_health - damage, 0.f, max_health);
	UpdateHealthProgress();

	if (current_health <= 0.f)
	{
		ASciFiCombatGameMode* scifi_cobat_gamemode = GetWorld()->GetAuthGameMode<ASciFiCombatGameMode>();
		if (scifi_cobat_gamemode)
		{
			scifi_combat_player_controller = scifi_combat_player_controller == nullptr ?
				Cast<ASciFiCombatPlayerController>(Controller) : scifi_combat_player_controller;
			ASciFiCombatPlayerController* attacker_controller = Cast<ASciFiCombatPlayerController>(instigator_controller);
			scifi_cobat_gamemode->PlayerEliminated(this, scifi_combat_player_controller,
				attacker_controller);
		}
	}
}

void ACombatCharacter::PlayDeathMontage()
{
	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (anim_instance && death_montage1 && death_montage2 && death_montage3)
	{
		// 이후에 랜덤한 로직을 추가해서 2번 3번 모션도 재생될수있게
		anim_instance->Montage_Play(death_montage1);
	}
}

void ACombatCharacter::DeathTimerFinished()
{
	ASciFiCombatGameMode* _game_mode = GetWorld()->GetAuthGameMode<ASciFiCombatGameMode>();
	if (_game_mode)
	{
		_game_mode->RequestRespawn(this, Controller);
	}
}

void ACombatCharacter::CallEliminate()
{
	if (combat_component && combat_component->equipped_weapon)
	{
		combat_component->equipped_weapon->DropWeapon();
	}
	Eliminate();
	GetWorldTimerManager().SetTimer(
		death_timer,
		this,
		&ACombatCharacter::DeathTimerFinished,
		death_delay
	);
}

void ACombatCharacter::Eliminate_Implementation()
{
	if (scifi_combat_player_controller)
	{
		scifi_combat_player_controller->SetWeaponAmmoText(0);
	}
	is_death = true;
	PlayDeathMontage();

	// 여기서 래그돌 이벤트

	// 여기서는 Movement 무력화
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	gameplay_disable_option = true;
	if (combat_component)
	{
		combat_component->PressFireKey(false);
	}
//	if (scifi_combat_player_controller)
//	{
//		DisableInput(scifi_combat_player_controller);
//	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (IsLocallyControlled() && combat_component && combat_component->isAim && 
		combat_component->equipped_weapon && combat_component->equipped_weapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		ShowSniperHUD(false);
	}
}

void ACombatCharacter::Destroyed()
{
	Super::Destroyed();

	ASciFiCombatGameMode* game_mode = Cast< ASciFiCombatGameMode>(UGameplayStatics::GetGameMode(this));
	bool is_game_progress = game_mode && game_mode->GetMatchState() != MatchState::InProgress;

	if (combat_component && combat_component->equipped_weapon && is_game_progress)
	{
		combat_component->equipped_weapon->Destroy();
	}
}

void ACombatCharacter::PressAbility1()
{
	if (GetIsCCControlled()) return;
	if (ManaCheck(50))
	{
		UseMainAbility();
	}
}

void ACombatCharacter::PressAbility2()
{
	if (GetIsCCControlled()) return;
	if (ManaCheck(25))
	{
		UseSubAbility();
	}
}

void ACombatCharacter::UseMainAbility()
{
	ability_component->ExecuteMainAbility(selected_character_class);
}

void ACombatCharacter::UseSubAbility()
{
	ability_component->ExecuteSubAbility(GetAvailableSubAbility());
}

void ACombatCharacter::PressAbility3()
{
	// Fuction Test
	//IncreaseMana(10);
}

void ACombatCharacter::PressAbility4()
{
	// Fuction Test
	//UseMana(10);
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			IncreaseMana(10);
		}), 2.0, false);
	FTimerHandle delay_handle2;
	GetWorld()->GetTimerManager().SetTimer(delay_handle2, FTimerDelegate::CreateLambda([&]()
		{
			Jump();
		}), 2.0, false);
}

void ACombatCharacter::Awakening()
{
	scifi_combat_player_controller = scifi_combat_player_controller == nullptr ?
		Cast<ASciFiCombatPlayerController>(Controller) : scifi_combat_player_controller;
	if (!scifi_combat_player_controller) { return; }
	selected_character_class = scifi_combat_player_controller->selected_character_class;
	SetAbilityTexture();
	ServerAwakening(selected_character_class);
}

void ACombatCharacter::SetWeaponImage()
{
	//if (combat_component->equipped_weapon)
	//{
	//	if (combat_component->equipped_weapon->weapon_image)
	//	{
	//		scifi_combat_player_controller->player_hud->status_overlay->weapon_image->SetBrushFromTexture(combat_component->equipped_weapon->weapon_image);
	//	}
	//}
}

void ACombatCharacter::SetAbilityTexture()
{
	if (selected_character_class == ECombatCharacterClass::ECC_CombatMaster)
	{
		//scifi_combat_player_controller->player_hud->status_overlay->main_skill_texture = main_skill_damage_immune_texture;
		scifi_combat_player_controller->player_hud->status_overlay->main_skill_image->SetBrushFromTexture(main_skill_teleport_texture);
	}
	else if (selected_character_class == ECombatCharacterClass::ECC_Magician)
	{
		//scifi_combat_player_controller->player_hud->status_overlay->main_skill_texture = main_skill_barrier_texture;
		scifi_combat_player_controller->player_hud->status_overlay->main_skill_image->SetBrushFromTexture(main_skill_barrier_texture);
	}
	else if (selected_character_class == ECombatCharacterClass::ECC_Guardian)
	{
		//scifi_combat_player_controller->player_hud->status_overlay->main_skill_texture = main_skill_teleport_texture;
		scifi_combat_player_controller->player_hud->status_overlay->main_skill_image->SetBrushFromTexture(main_skill_damage_immune_texture);
	}
}

void ACombatCharacter::ServerAwakening_Implementation(ECombatCharacterClass selected_class)
{
	//int32 dice_min = 0;
	//int32 dice_max = 2;
	//int32 dice = FMath::RandRange(dice_min, dice_max);

	//if (selected_character_class == ECombatCharacterClass::ECC_CombatMaster)
	//{
	//	GetMesh()->SetSkeletalMesh(ability_component->combat_master_mesh);
	//}
	//else if (selected_character_class == ECombatCharacterClass::ECC_Magician)
	//{
	//	GetMesh()->SetSkeletalMesh(ability_component->megician_mesh);
	//}
	//else if (selected_character_class == ECombatCharacterClass::ECC_Guardian)
	//{
	//	GetMesh()->SetSkeletalMesh(ability_component->guardian_mesh);
	//}
	NetMulticastAwakening(selected_class);
}

void ACombatCharacter::NetMulticastAwakening_Implementation(ECombatCharacterClass selected_class)
{
	//int32 dice_min = 0;
	//int32 dice_max = 2;
	//int32 dice = FMath::RandRange(dice_min, dice_max);

	if (selected_class == ECombatCharacterClass::ECC_CombatMaster)
	{
		GetMesh()->SetSkeletalMesh(ability_component->combat_master_mesh);
	}
	else if (selected_class == ECombatCharacterClass::ECC_Magician)
	{
		GetMesh()->SetSkeletalMesh(ability_component->megician_mesh);
	}
	else if (selected_class == ECombatCharacterClass::ECC_Guardian)
	{
		GetMesh()->SetSkeletalMesh(ability_component->guardian_mesh);
	}
}

void ACombatCharacter::PressEvadeKey()
{
	if (GetInputLock()) return;

	CallEvadeFunc();
}

void ACombatCharacter::CallEvadeFunc()
{
	if (HasAuthority())
	{
		if (is_sprint)
		{
			ServerBlink();
		}
		else
		{
			ServerEvade();
		}

	}
	else
	{
		if (is_sprint)
		{
			ServerBlink();
		}
		else
		{
			ServerEvade();
		}
	}

}


void ACombatCharacter::TeleportCharacter(FVector loc)
{
	ServerTeleportCharacter(loc);
}

void ACombatCharacter::ServerTeleportCharacter_Implementation(FVector loc)
{
	MultiCastTeleportCharacter(loc);
}

void ACombatCharacter::MultiCastTeleportCharacter_Implementation(FVector loc)
{
	SetActorLocation(loc);
}

void ACombatCharacter::EvadeUpdate()
{
	// Depercated
}

void ACombatCharacter::EvadeFinished()
{
	// Depercated
}

void ACombatCharacter::ServerBlink_Implementation()
{
	MultiCastBlink();
}

void ACombatCharacter::MultiCastBlink_Implementation()
{
	// 이펙트 생성
	if (blink_effect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			blink_effect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	// 애니메이션 실행
	if (blink_anim_sequence)
	{
		GetMesh()->PlayAnimation(blink_anim_sequence, false);
		// Delay
		FTimerHandle wait_handle;
		GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
			{
				GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}), blink_anim_sequence->GetPlayLength() - blink_anim_offset, false);
	}
	// SetLocation
	FVector blink_loc = GetActorForwardVector();
	blink_loc.X += blink_anim_offset;
	SetActorLocation(blink_loc, true);
}

void ACombatCharacter::ServerEvade_Implementation()
{
	MultiCastEvade();
}

void ACombatCharacter::MultiCastEvade_Implementation()
{
	FVector evade_foward_velocity = UKismetMathLibrary::GetForwardVector(GetActorRotation()) * evade_distance;
	GetMesh()->PlayAnimation(evade_anim_sequence, false);
	GetCharacterMovement()->AddForce(evade_foward_velocity.GetSafeNormal(0.0001) * evade_force);

	// Delay
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			// 여기에 코드를 치면 된다.
		}), evade_delay, false); //반복도 여기서 추가 변수를 선언해 설정가능
	// Delay
}

void ACombatCharacter::ForceMoveCharacter(float launch_distance, float launch_force)
{
	FVector launch_velocity = UKismetMathLibrary::GetForwardVector(GetActorRotation()) * launch_distance;
	GetCharacterMovement()->AddForce(launch_velocity.GetSafeNormal(0.0001) * launch_force);
}

void ACombatCharacter::Evade()
{
	// Depercated
}

void ACombatCharacter::SetDamageImmune(bool value)
{
	ServerSetDamageImmune(value);
}
void ACombatCharacter::ServerSetDamageImmune_Implementation(bool value)
{
	MultiCastSetDamageImmune(value);
}
void ACombatCharacter::MultiCastSetDamageImmune_Implementation(bool value)
{
	if (value)
	{
		if (ability_component->damage_immune_effect_niagara)
		{
			//UNiagaraFunctionLibrary::SpawnSystemAttached(blasterAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
			UNiagaraFunctionLibrary::SpawnSystemAttached(ability_component->damage_immune_effect_niagara, RootComponent, FName("None"),
				GetActorLocation(), GetActorRotation(), EAttachLocation::Type::KeepWorldPosition, false);
		}
	}
	else
	{
		//if (ability_component->damage_immune_effect_niagara)
		//{
		//	ability_component->damage_immune_effect_niagara->RemoveFromRoot();
		//}
	}
	damage_immune = value;
}

bool ACombatCharacter::GetIsCCControlled()
{
	return cc_component->GetIsCrowdControlled();
}


bool ACombatCharacter::ManaCheck(float need_mana)
{
	// true일 경우 마나를 깎고 실행, false면 실행이안됨
	if (current_mana >= need_mana)
	{
		UseMana(need_mana);
		//UpdateManaProgress();
		return true;
	}
	BeepAbility();
	SetAbilityAlarmText("Not Enough Mana");
	return false;
}

bool ACombatCharacter::SmashPowerCheck(float need_power)
{
	// true일 경우 스매시 파워를 깎고 실행, false면 실행이안됨
	if (current_smash_power >= need_power)
	{
		UseSmashPower();
		//UpdateSmashPowerProgress();
		return true;
	}
	BeepAbility();
	SetAbilityAlarmText("Not Enough Smash Power");
	return false;
}

void ACombatCharacter::SetAbilityAlarmText(FString alram_str)
{
	scifi_combat_player_controller->SetAbilityAlarmText(alram_str);
}

void ACombatCharacter::BeepAbility()
{
	if (beep_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			beep_sound,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}

void ACombatCharacter::InteractPickupItem()
{
	inventory_component->InteractPickupItem();
}

FVector ACombatCharacter::GetFollowCameraLocation()  
{ 
	return follow_camera->GetComponentLocation(); 
}

FVector ACombatCharacter::GetFollowCameraForwardVector()
{
	return follow_camera->GetForwardVector();
}


void ACombatCharacter::CallEmotion(const FString& emotion)
{
	ServerCallEmotion(emotion);
}

void ACombatCharacter::ServerCallEmotion_Implementation(const FString& emotion)
{
	MultiCastCallEmotion(emotion);
}

void ACombatCharacter::MultiCastCallEmotion_Implementation(const FString& emotion)
{
	if (emotion == "dance")
	{
		PlayAnimSequence(dance_anim_sequence);
		ResetAnimMode(dance_delay);
	}
	else if (emotion == "lol")
	{
		PlayAnimSequence(lol_anim_sequence);
		ResetAnimMode(lol_delay);
	}
	else if (emotion == "meditation")
	{
		PlayAnimSequence(meditation_anim_sequence);
		ResetAnimMode(meditation_delay);
	}
}

void ACombatCharacter::PlayAnimSequence(class UAnimationAsset* animation_asset)
{
	GetMesh()->PlayAnimation(animation_asset, false);
}

void ACombatCharacter::ResetAnimMode(float delay)
{
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}), delay, false);
}

void ACombatCharacter::OnSprint()
{
	if (HasAuthority())
	{
		ServerOnSprint();
	}
	else
	{
		ServerOnSprint();
	}
}

void ACombatCharacter::ServerOnSprint_Implementation()
{
	NetMulticastOnSprint();
}

void ACombatCharacter::NetMulticastOnSprint_Implementation()
{
	is_sprint = true;
	GetCharacterMovement()->MaxWalkSpeed = sprint_speed;
}

void ACombatCharacter::OffSprint()
{
	if (HasAuthority())
	{
		ServerOffSprint();
	}
	else
	{
		ServerOffSprint();
	}
}

void ACombatCharacter::ServerOffSprint_Implementation()
{
	NetMulticastOffSprint();
}

void ACombatCharacter::NetMulticastOffSprint_Implementation()
{
	is_sprint = false;
	GetCharacterMovement()->MaxWalkSpeed = sprint_reset_speed;
}

void ACombatCharacter::PressGuard()
{

	if (combat_component->equipped_weapon->weapon_style == EWeaponStyle::WST_Melee)
	{
		// 밀리 무기를 장착중인지 확인
		if (HasAuthority())
		{
			GuardAction();
		}
		else
		{
			GuardAction();
		}
	}
}

void ACombatCharacter::ReleaseGuard()
{

	if (combat_component->equipped_weapon->weapon_style == EWeaponStyle::WST_Melee)
	{
		// 밀리 무기를 장착중인지 확인
		if (HasAuthority())
		{
			GuardCancel();
		}
		else
		{
			GuardCancel();
		}
	}
}

void ACombatCharacter::GuardAction()
{
	SetInputLock(true);
	ServerGuardAction();
}

void ACombatCharacter::ServerGuardAction_Implementation()
{
	MulticastGuardAction();
}

void ACombatCharacter::MulticastGuardAction_Implementation()
{
	is_guard = true;
}

void ACombatCharacter::GuardCancel()
{
	SetInputLock(false);
	ServerGuardCancel();
}

void ACombatCharacter::ServerGuardCancel_Implementation()
{
	MulticastGuardCancel();
}

void ACombatCharacter::MulticastGuardCancel_Implementation()
{
	is_guard = false;
}

/*
Cast<ACombatCharacter>(GetOwner())->GetMesh()->PlayAnimation(swing_anim_sequence2, false);

void ACombatSpear::ResetAnimMode(float delay)
{
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			weapon_owner_character->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}), delay, false);
}
*/



// Depercated -*-------------------------------------------------------*- Depercated // 
void ACombatCharacter::CallPressGrappleEvent()
{
	if (HasAuthority())
	{
		ServerFireGrappleHook();
	}
	else
	{
		ServerFireGrappleHook();
	}
}

void ACombatCharacter::ServerFireGrappleHook_Implementation()
{
	MultiCastFireGrappleHook();
}

void ACombatCharacter::MultiCastFireGrappleHook_Implementation()
{

}

void ACombatCharacter::FireGrappleHook()
{

}

void ACombatCharacter::CallReleaseGrappleEvent()
{

}

void ACombatCharacter::ServerReleaseGrappleHook_Implementation()
{
	MultiCastReleaseGrappleHook();
}

void ACombatCharacter::MultiCastReleaseGrappleHook_Implementation()
{

}

void ACombatCharacter::ReleaseGrappleHook()
{

}



