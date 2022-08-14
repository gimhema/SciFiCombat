// Fill out your copyright notice in the Description page of Project Settings.



#include "WireAction/GrappleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "CableComponent.h"
#include "SciFiCombat/Public/WireAction/GrappleHook.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UGrappleComponent::UGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrappleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GrappleStateLoop();
	// ...
}

void UGrappleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGrappleComponent, grapple_state);
	DOREPLIFETIME(UGrappleComponent, grapple_hook);
	DOREPLIFETIME(UGrappleComponent, grapple_cable_actor);
}


void UGrappleComponent::GrappleStateLoop()
{
	if (grapple_owner && grapple_hook)
	{
		FVector _force = ToGrappleHook() * 100000.f;

		float _distance = FVector::Distance(
			grapple_hook->GetActorLocation(),
			grapple_owner->GetActorLocation()
		);

		switch (grapple_state)
		{
		case EGrappleState::EGS_ReadyToFire:
			break;
		case EGrappleState::EGS_Firing:
			break;
		case EGrappleState::EGS_AttachToTarget:	
			grapple_owner->GetCharacterMovement()->AddForce(_force);
			if (_distance < 250.f)
			{
				if (grapple_hook)
				{
					grapple_hook->Destroy();
				}
			}
			else
			{
				float _dot = FVector::DotProduct(InitiailHookDirection2D, ToGrappleHook2D());
				if (grapple_hook)
				{
					grapple_hook->Destroy();
				}
			}
			break;
		}
	}

}

bool UGrappleComponent::isInUse()
{
	switch (grapple_state)
	{
	case EGrappleState::EGS_AttachToTarget:
		return true;
		break;
	case EGrappleState::EGS_Firing:
		return true;
		break;
	case EGrappleState::EGS_ReadyToFire:
		return false;
		break;
	}
	return false;
}

void UGrappleComponent::FireGrapple(FVector target_location, FVector local_offset)
{
	//if (HasAuthority())
	if (!isInUse() && grapple_owner && grapple_owner->HasAuthority())
	{
		grapple_state = EGrappleState::EGS_Firing;

		FVector fire_direction_calc_temp = target_location - CableStartLocation(local_offset);
		FVector fire_direction = fire_direction_calc_temp.GetSafeNormal(0.0001);

		// APawn* instigator = Cast<APawn>(GetOwner());

			if (grapple_hook_class && grapple_cable_actor_class)
			{
				FActorSpawnParameters spawn_params;
				spawn_params.Owner = GetOwner();
				// spawn_params.Instigator = instigator;
				UWorld* world = GetWorld();
	
				FVector spawn_location = CableStartLocation(local_offset);
				FRotator spawn_rot = FRotator(0.0, 0.0, 0.0);
			
				if (world)
				{
					grapple_hook = world->SpawnActor<AGrappleHook>(
						grapple_hook_class,
						spawn_location,
						spawn_rot,
						spawn_params
						);
					//grapple_hook->hook_veloicty = fire_direction * speed;
					grapple_hook->SetGrappleComponent(this);

					FRotator cable_spawn_rot = UKismetMathLibrary::MakeRotFromX(fire_direction);
					grapple_cable_actor = world->SpawnActor<AGrappleCableActor>(
						grapple_cable_actor_class,
						spawn_location,
						cable_spawn_rot,
						spawn_params
						);

					

					//grapple_cable_actor->AttachToActor(grapple_owner, attachment_rules, FName(""));
					const USkeletalMeshSocket* fire_grapple_socket = grapple_owner->GetMesh()->GetSocketByName(FName("FireGrappleSocket"));
					if (fire_grapple_socket)
					{
						fire_grapple_socket->AttachActor(grapple_cable_actor, grapple_owner->GetMesh());
					}

					grapple_cable_actor->CableComponent->SetAttachEndTo(grapple_hook, FName("None"), FName("None"));
					grapple_cable_actor->CableComponent->EndLocation.X = 0.f;
					grapple_cable_actor->CableComponent->EndLocation.Y = 0.f;
					grapple_cable_actor->CableComponent->EndLocation.Z = 0.f;
				}
			}

	}
}

void UGrappleComponent::CancelGrapple()
{
	if (grapple_hook && grapple_owner && grapple_owner->HasAuthority())
	{
		grapple_hook->Destroy();
	}
}

FVector UGrappleComponent::CableStartLocation(FVector direction)
{
	FVector result;

	FVector owner_loc = grapple_owner->GetActorLocation();
	FVector owner_direction = UKismetMathLibrary::TransformDirection(grapple_owner->GetActorTransform(), direction);
	result = owner_loc + owner_direction;

	return result;
}

FVector UGrappleComponent::ToGrappleHook()
{
	FVector _temp = grapple_hook->GetActorLocation() - grapple_owner->GetActorLocation();
	FVector result = _temp.GetSafeNormal(0.0001);

	return result;
}

FVector UGrappleComponent::ToGrappleHook2D()
{
	FVector temp = ToGrappleHook();
	return FVector(temp.X, temp.Y, 0.f);
}

void UGrappleComponent::ServerFireGrapple_Implementation(FVector target_location, FVector local_offset)
{
	MultiCastFireGrapple(target_location, local_offset);
}

void UGrappleComponent::MultiCastFireGrapple_Implementation(FVector target_location, FVector local_offset)
{
	FireGrapple(target_location, local_offset);
}

void UGrappleComponent::ServerCancelGrapple_Implementation()
{
	MultiCastCancelGrapple();
}

void UGrappleComponent::MultiCastCancelGrapple_Implementation()
{
	CancelGrapple();
}


/*
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
*/