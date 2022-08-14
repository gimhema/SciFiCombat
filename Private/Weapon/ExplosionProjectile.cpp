// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ExplosionProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AExplosionProjectile::AExplosionProjectile()
{
	missile_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileMesh"));
	missile_mesh->SetupAttachment(RootComponent);
	missile_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	missile_sk_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MissileSKMesh"));
	missile_sk_mesh->SetupAttachment(RootComponent);
	missile_sk_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AExplosionProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImulse, const FHitResult& Hit)
{
	APawn* shooter = GetInstigator();
	if (!is_bounce)
	{
		if (shooter)
		{
			AController* shooter_controller = shooter->GetController();
			if (shooter_controller)
			{
				UGameplayStatics::ApplyRadialDamageWithFalloff(
					this,
					projectile_damage,
					10.f,
					GetActorLocation(),
					200.f,
					500.f,
					1.f,
					UDamageType::StaticClass(),
					TArray<AActor*>(),
					this,
					shooter_controller
				);
			}
		}
		Super::OnHit(HitComp, OtherActor, OtherComp, NormalImulse, Hit);
	}
	else
	{
		BombTimerStart();
	}
}

void AExplosionProjectile::BombTimerStart()
{
	GetWorldTimerManager().SetTimer(
		bomb_timer_handle,
		this,
		&AExplosionProjectile::BombTimerEnd,
		bomb_time
	);
}

void AExplosionProjectile::BombTimerEnd()
{
	APawn* shooter = GetInstigator();

	if (shooter)
	{
		AController* shooter_controller = shooter->GetController();
		if (shooter_controller)
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this,
				projectile_damage,
				10.f,
				GetActorLocation(),
				200.f,
				500.f,
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				this,
				shooter_controller
			);
		}
	}
	Destroy();
}
