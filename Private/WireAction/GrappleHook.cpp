// Fill out your copyright notice in the Description page of Project Settings.


#include "WireAction/GrappleHook.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGrappleHook::AGrappleHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//hit_area = CreateDefaultSubobject<UBoxComponent>(TEXT("HitArea"));
	//SetRootComponent(hit_area);
	//hit_area->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//hit_area->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//hit_area->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	//hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	//hit_area->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);
	hook_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookMesh"));
	SetRootComponent(hook_mesh);
	projectile_movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	projectile_movement->bRotationFollowsVelocity = true;

}

// Called when the game starts or when spawned
void AGrappleHook::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		hook_mesh->OnComponentHit.AddDynamic(this, &AGrappleHook::OnGrappleHit);

		//projectile_movement->Velocity = hook_veloicty;
		start_location = GetActorLocation();
	}
}

// Called every frame
void AGrappleHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		float dist = FVector::Distance(start_location, GetActorLocation());
		if (dist >= max_distance)
		{
			Destroy();
		}
	}
}


void AGrappleHook::Destroyed()
{
	Super::Destroyed();
	if (grapple_component)
	{
		grapple_component->grapple_hook = nullptr;
		grapple_component->grapple_state = EGrappleState::EGS_ReadyToFire;
		
		if (grapple_component->grapple_cable_actor)
		{
			grapple_component->grapple_cable_actor->Destroy();
			grapple_component->grapple_cable_actor = nullptr;
		}
		grapple_component->grapple_owner->GetCharacterMovement()->GroundFriction = 1.0f;
		grapple_component->grapple_owner->GetCharacterMovement()->GravityScale = 1.0f;
		grapple_component->grapple_owner->GetCharacterMovement()->AirControl = 0.35;
	}
}

void AGrappleHook::OnGrappleHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (grapple_component)
	{
		grapple_component->grapple_state = EGrappleState::EGS_AttachToTarget;
		grapple_component->grapple_owner->GetCharacterMovement()->GroundFriction = 0.f;
		grapple_component->grapple_owner->GetCharacterMovement()->GravityScale = 0.f;
		grapple_component->grapple_owner->GetCharacterMovement()->AirControl = 0.2;

		grapple_component->grapple_owner->GetCharacterMovement()->Velocity = grapple_component->ToGrappleHook() * 1200.f;
		grapple_component->InitiailHookDirection2D = grapple_component->ToGrappleHook2D();
	}
}
