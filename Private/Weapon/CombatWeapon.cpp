// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CombatWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "Animation/AnimationAsset.h"
#include "Net/UnrealNetwork.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"

ACombatWeapon::ACombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	weapon_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(weapon_mesh);

	weapon_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	weapon_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	weapon_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	weapon_mesh->SetCustomDepthStencilValue(outline_depth_color);
	weapon_mesh->MarkRenderStateDirty();
	EnableCustomDepth(true);

	weapon_area = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponArea"));
	weapon_area->SetupAttachment(RootComponent);
	weapon_area->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	weapon_area->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	pickup_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WeaponPickupWidget"));
	pickup_widget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACombatWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) // Same Meaning -> GetLocalRole() == ENetRole::ROLE_Authority
	{
		weapon_area->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		weapon_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		weapon_area->OnComponentBeginOverlap.AddDynamic(this, &ACombatWeapon::OnSphereOverlap);
		weapon_area->OnComponentEndOverlap.AddDynamic(this, &ACombatWeapon::OnSphereEndOverlap);
	}
	if (pickup_widget)
	{
		pickup_widget->SetVisibility(false);
	}
}

void ACombatWeapon::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult
)
{
	ACombatCharacter* combat_character = Cast<ACombatCharacter>(OtherActor);
	if (combat_character)
	{
		//pickup_widget->SetVisibility(true);
		combat_character->SetOverlappingWeapon(this);
	}
}


void ACombatWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(ACombatWeapon, weapon_status);
	DOREPLIFETIME(ACombatWeapon, ammo);
	DOREPLIFETIME(ACombatWeapon, melee_combo_count);
	DOREPLIFETIME(ACombatWeapon, melee_hit_enable);
	DOREPLIFETIME(ACombatWeapon, melee_hit_enable);
	DOREPLIFETIME(ACombatWeapon, weapon_image);
	DOREPLIFETIME(ACombatWeapon, is_can_melee_attack);
}

void ACombatWeapon::OnRep_Ammo()
{
	weapon_owner_character = weapon_owner_character == nullptr ? Cast<ACombatCharacter>(GetOwner()) : weapon_owner_character;
	SetAmmoText();
}

void ACombatWeapon::SpendRound()
{
	ammo = FMath::Clamp(ammo - 1, 0, max_ammo);
	SetAmmoText();
}

void ACombatWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACombatCharacter* combat_character = Cast<ACombatCharacter>(OtherActor);
	if (combat_character)
	{
		combat_character->SetOverlappingWeapon(nullptr);
	}
}
void ACombatWeapon::EnableCustomDepth(bool is_enable)
{
	if (weapon_mesh)
	{
		weapon_mesh->SetRenderCustomDepth(is_enable);
	}
}
void ACombatWeapon::OnRep_WeaponStatus()
{
	OnWeaponStateSet();
	/*switch (weapon_status)
	{
	case EWeaponStatus::EWS_Equipped:
		ShowPickUpWeaponWidget(false);
		weapon_mesh->SetSimulatePhysics(false);
		weapon_mesh->SetEnableGravity(false);
		weapon_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EnableCustomDepth(false);

		break;
	case EWeaponStatus::EWS_Dropped:
		weapon_mesh->SetSimulatePhysics(true);
		weapon_mesh->SetEnableGravity(true);
		weapon_mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		weapon_mesh->SetCustomDepthStencilValue(outline_depth_color);
		weapon_mesh->MarkRenderStateDirty();
		EnableCustomDepth(true);

		break;
	}*/
}

void ACombatWeapon::ShowPickUpWeaponWidget(bool isShowWidget)
{
	if (pickup_widget)
	{
		pickup_widget->SetVisibility(isShowWidget);
	}
}


void ACombatWeapon::DropWeapon()
{
	SetWeaponStatus(EWeaponStatus::EWS_Dropped);
	FDetachmentTransformRules detach_rule(EDetachmentRule::KeepWorld, true);
	weapon_mesh->DetachFromComponent(detach_rule);
	SetOwner(nullptr);
	weapon_owner_character = nullptr;
	weapon_owner_controller = nullptr;
}

void ACombatWeapon::SetWeaponStatus(EWeaponStatus status)
{
	weapon_status = status;
	OnWeaponStateSet();
}


void ACombatWeapon::OnWeaponStateSet()
{
	switch (weapon_status)
	{
	case EWeaponStatus::EWS_Equipped:
		OnEquipped();
		break;
	case EWeaponStatus::EWS_Dropped:
		OnDropped();
		break;
	case EWeaponStatus::EWS_EquippedSub:
		OnEquippedSub();
		break;
	}
}

void ACombatWeapon::OnEquipped()
{
	ShowPickUpWeaponWidget(false);
	weapon_area->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	weapon_mesh->SetSimulatePhysics(false);
	weapon_mesh->SetEnableGravity(false);
	weapon_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnableCustomDepth(false);
}
void ACombatWeapon::OnDropped()
{
	if (HasAuthority())
	{
		weapon_area->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	weapon_mesh->SetSimulatePhysics(true);
	weapon_mesh->SetEnableGravity(true);
	weapon_mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	weapon_mesh->SetCustomDepthStencilValue(outline_depth_color);
	weapon_mesh->MarkRenderStateDirty();
	EnableCustomDepth(true);
}

void ACombatWeapon::OnEquippedSub()
{
	ShowPickUpWeaponWidget(false);
	weapon_area->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	weapon_mesh->SetSimulatePhysics(false);
	weapon_mesh->SetEnableGravity(false);
	weapon_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnableCustomDepth(false);
}

void ACombatWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	if (Owner == nullptr)
	{
		weapon_owner_character = nullptr;
		weapon_owner_controller = nullptr;
	}
	else
	{
		SetAmmoText();
	}
}

bool ACombatWeapon::isWeaponEmpty()
{
	return ammo <= 0;
}

void ACombatWeapon::SetAmmoText()
{
	weapon_owner_character = weapon_owner_character == nullptr ? Cast<ACombatCharacter>(GetOwner()) : weapon_owner_character;
	if (weapon_owner_character)
	{
		weapon_owner_controller = weapon_owner_controller == nullptr ? Cast<ASciFiCombatPlayerController>(weapon_owner_character->Controller) : weapon_owner_controller;
		if (weapon_owner_controller)
		{
			weapon_owner_controller->SetWeaponAmmoText(ammo);
		}
	}
}

void ACombatWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatWeapon::Fire(const FVector& hit_target)
{
	if (fire_animation)
	{
		weapon_mesh->PlayAnimation(fire_animation, false);
	}
	SpendRound();
}

void ACombatWeapon::ComboProcess()
{

}

void ACombatWeapon::MeleeSmashAttack()
{
	weapon_owner_character->UseSmashPower();
}

void ACombatWeapon::AddAmmo(int32 added_ammo)
{
	ammo = FMath::Clamp(ammo - added_ammo, 0, max_ammo);

	
}

void ACombatWeapon::SetWeaponOwnerInputLock(bool lock)
{
	weapon_owner_character->SetInputLock(lock);
}

bool ACombatWeapon::GetWeaponOwnerInputLock()
{
	return weapon_owner_character->GetInputLock();
}
