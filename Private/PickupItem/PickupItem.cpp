// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/PickupItem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	pick_area = CreateDefaultSubobject<USphereComponent>(TEXT("Pick Area"));
	pick_area->SetupAttachment(RootComponent);
	pick_area->SetSphereRadius(150.f);
	pick_area->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	pick_area->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	pick_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	pick_area->AddLocalOffset(FVector(0.f, 0.f, 85.f));

	pickup_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	pickup_mesh->SetupAttachment(pick_area);
	pickup_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	pickup_mesh->SetRenderCustomDepth(true);
	pickup_mesh->SetCustomDepthStencilValue(outline_color);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			bind_overlap_timer,
			this,
			&APickupItem::BindOverlapTimerFinished,
			bind_overlap_time
		);
	}
}

void APickupItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool isFromSweep, const FHitResult& SweepResult)
{

}

void APickupItem::Destroyed()
{
	Super::Destroyed();

	if (pickup_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			pickup_sound,
			GetActorLocation()
		);
	}

}


// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (pickup_mesh)
	{
		pickup_mesh->AddLocalRotation(FRotator(0.f, base_turn_rate * DeltaTime, 0.f));
	}

}

void APickupItem::BindOverlapTimerFinished()
{
	pick_area->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnSphereOverlap);
}