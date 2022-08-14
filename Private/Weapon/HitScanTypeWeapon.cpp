// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/HitScanTypeWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"

void AHitScanTypeWeapon::Fire(const FVector& hit_target)
{
	Super::Fire(hit_target);

	APawn* weapon_owner_pawn = Cast<APawn>(GetOwner());
	if (weapon_owner_pawn == nullptr) return;
	AController* instigator_controller = weapon_owner_pawn->GetController();

	const USkeletalMeshSocket* muzzle_flash = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (muzzle_flash && instigator_controller)
	{
		FTransform socket_transform = muzzle_flash->GetSocketTransform(GetWeaponMesh());
		FVector fire_start = socket_transform.GetLocation();

		FHitResult fire_hit;
		WeaponTraceHit(fire_start, hit_target, fire_hit);

		//ACombatCharacter* combat_character = Cast<ACombatCharacter>(fire_hit.GetActor());
		AMonsterBase* monster = Cast<AMonsterBase>(fire_hit.GetActor());

		if (monster && HasAuthority() && instigator_controller)
		{
			UGameplayStatics::ApplyDamage(
				monster,
				hit_scan_damage,
				instigator_controller,
				this,
				UDamageType::StaticClass()
			);
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
}

FVector AHitScanTypeWeapon::FireEndScatter(const FVector& fire_start, const FVector& hit_target)
{
	FVector target_normalized = (hit_target - fire_start).GetSafeNormal();
	FVector fire_center = fire_start + target_normalized * distance_sphere;
	FVector random_vector = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, sphere_radius);
	FVector fire_end = fire_center + random_vector;
	FVector to_fire_end = fire_end - fire_start;
	// Draw Debug
	// DrawDebugSphere(GetWorld(), fire_center, sphere_radius, 12, FColor::Red, true);
	// DrawDebugSphere(GetWorld(), fire_end, 4.f, 12, FColor::Orange, true);
	// DrawDebugLine(
	//	GetWorld(),
	//	fire_start,
	//	FVector(fire_start + to_fire_end * 80000.f / to_fire_end.Size()),
	//	FColor::Blue,
	//	true
	// );
	// 80000.f
	
	return FVector(fire_start + to_fire_end * 80000.f / to_fire_end.Size());
}

void AHitScanTypeWeapon::WeaponTraceHit(const FVector& fire_start, const FVector& hit_target, FHitResult& out_hit)
{
	// FHitResult fire_hit;
	UWorld* world = GetWorld();
	if (world)
	{
		FVector fire_end = is_scatter ? FireEndScatter(fire_start, hit_target) : fire_start + (hit_target - fire_start) * 1.25f;
		world->LineTraceSingleByChannel(
			out_hit,
			fire_start,
			fire_end,
			ECollisionChannel::ECC_Visibility
		);
		FVector effect_end = fire_end;
		if (out_hit.bBlockingHit)
		{
			effect_end = out_hit.ImpactPoint;
		}
		if (line_trace_effect)
		{
			UParticleSystemComponent* _impact = UGameplayStatics::SpawnEmitterAtLocation(
				world,
				line_trace_effect,
				fire_start,
				FRotator::ZeroRotator
			);
			if (_impact)
			{
				_impact->SetVectorParameter(FName("Target"), effect_end);
			}
		}
	}

}
