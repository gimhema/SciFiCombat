// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileTypeWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "SciFiCombat/Public/Weapon/ProjectileBase.h"

void AProjectileTypeWeapon::Fire(const FVector& hit_target)
{
	Super::Fire(hit_target);

	if (!HasAuthority()) return;

	APawn* instigator = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* muzzle_socket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	if (muzzle_socket)
	{
		FTransform socket_transform = muzzle_socket->GetSocketTransform(GetWeaponMesh());
		FVector target_vec = hit_target - socket_transform.GetLocation();
		FRotator target_rot = target_vec.Rotation();
		if (projectile_class && instigator)
		{
			FActorSpawnParameters spawn_params;
			spawn_params.Owner = GetOwner();
			spawn_params.Instigator = instigator;
			UWorld* world = GetWorld();
			if (world)
			{
				world->SpawnActor<AProjectileBase>(
					projectile_class,
					socket_transform.GetLocation(),
					target_rot,
					spawn_params
					);
			}
		}
	}
}




