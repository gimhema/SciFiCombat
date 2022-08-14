#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	EWT_MissileLauncher UMETA(DisplayName = "Missile Launcher"),
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),
	EWT_ShotGun UMETA(DisplayName = "ShotGun"),
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),
	EWT_TerribleAxe UMETA(DisplayName = "Terrible Axe"),
	EWT_GreatSword UMETA(DisplayName = "Great Sword"),
	EWT_CombatSpear UMETA(DisplayName = "Combat Spear"),
	EWT_MAX UMETA(DisplayName = "DefaultMax")
};
