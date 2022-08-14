// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BulletProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SciFiCombat/Public/Character/Turret.h"

void ABulletProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImulse, const FHitResult& Hit)
{
	ACharacter* owner_character = Cast<ACharacter>(GetOwner());
	if (owner_character)
	{
		AController* owner_controller = owner_character->Controller;
		if (owner_controller)
		{
			UGameplayStatics::ApplyDamage(OtherActor, projectile_damage, owner_controller, this, UDamageType::StaticClass());
		}
	}
	else
	{
		ATurret* turret = Cast<ATurret>(GetOwner());
		//AController* turret_controller = turret->Controller;
		UGameplayStatics::ApplyDamage(OtherActor, projectile_damage, nullptr, this, UDamageType::StaticClass());
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImulse, Hit);
}




