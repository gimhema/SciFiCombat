// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ScatterTypeWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"

void AScatterTypeWeapon::Fire(const FVector& hit_target)
{
	ACombatWeapon::Fire(hit_target);
	APawn* weapon_owner_pawn = Cast<APawn>(GetOwner());
	if (weapon_owner_pawn == nullptr) return;
	AController* instigator_controller = weapon_owner_pawn->GetController();

	const USkeletalMeshSocket* muzzle_flash = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (muzzle_flash && instigator_controller)
	{
		FTransform socket_transform = muzzle_flash->GetSocketTransform(GetWeaponMesh());
		FVector fire_start = socket_transform.GetLocation();
		TMap<AMonsterBase*, uint32> hit_map;
		for (uint32 i = 0; i < num_scatter_bullets; i++)
		{
			// FVector fire_end = FireEndScatter(fire_start, hit_target);
			FHitResult fire_hit;
			WeaponTraceHit(fire_start, hit_target, fire_hit);

			//ACombatCharacter* combat_character = Cast<ACombatCharacter>(fire_hit.GetActor());
			 AMonsterBase* monster = Cast<AMonsterBase>(fire_hit.GetActor());

			if (monster && HasAuthority() && instigator_controller)
			{
				if (hit_map.Contains(monster))
				{
					hit_map[monster]++;
				}
				else
				{
					hit_map.Emplace(monster, 1);
				}
			}
			if (hit_impact)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					hit_impact,
					fire_hit.ImpactPoint,
					fire_hit.ImpactNormal.Rotation(),
					impact_scale
				);
			}
		}
		for (auto hit_pair : hit_map)
		{
			if (hit_pair.Key && HasAuthority() && instigator_controller)
			{
				UGameplayStatics::ApplyDamage(
					hit_pair.Key,
					hit_scan_damage * hit_pair.Value,
					instigator_controller,
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
}

