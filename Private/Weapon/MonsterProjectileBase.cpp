// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MonsterProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"

void AMonsterProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImulse, const FHitResult& Hit)
{
	bool target_player_valid = IsValid(Cast<ACombatCharacter>(OtherActor));

	if (target_player_valid)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Player Valid"));
		UGameplayStatics::ApplyDamage(OtherActor, projectile_damage, nullptr, this, UDamageType::StaticClass());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Player Not Valid"));
		return;
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImulse, Hit);
}