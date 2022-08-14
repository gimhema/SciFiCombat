// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ProjectileBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/SciFiCombat.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	hit_area = CreateDefaultSubobject<UBoxComponent>(TEXT("HitArea"));
	SetRootComponent(hit_area);
	hit_area->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	hit_area->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	hit_area->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	hit_area->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);
	projectile_movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	projectile_movement->bRotationFollowsVelocity = true;

	trail_scale.X = 1.0;
	trail_scale.Y = 1.0;
	trail_scale.Z = 1.0;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (projectile_trail)
	{
		trail_component = UGameplayStatics::SpawnEmitterAttached(
			projectile_trail,
			hit_area,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			trail_scale,
			EAttachLocation::KeepWorldPosition
		);
	}

	if (HasAuthority())
	{
		hit_area->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImulse, const FHitResult& Hit)
{
	Destroy();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::Destroyed()
{
	Super::Destroyed();
	if (hit_particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			hit_particle,
			GetActorTransform()
		);
	}
	if (hit_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			hit_sound,
			GetActorLocation()
		);
	}
}

void AProjectileBase::SpawnTrailNiagaraSystem()
{
}




