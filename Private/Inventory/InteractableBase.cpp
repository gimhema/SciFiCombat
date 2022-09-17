// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InteractableBase.h"
#include "Inventory/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Character/CombatCharacter.h"
#include "Components/WidgetComponent.h"
// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	pickup_mesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	SetRootComponent(pickup_mesh);

	item_collection_sphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollectionSphere"));
	item_collection_sphere->SetupAttachment(RootComponent);
	item_collection_sphere->SetSphereRadius(200.f);

	pickup_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	pickup_widget->SetupAttachment(RootComponent);

	name = "Interactable";
	action = "interact";
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	

	if (HasAuthority())
	{
		item_collection_sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		item_collection_sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		item_collection_sphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableBase::OnSphereOverlap);
		item_collection_sphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableBase::OnSphereEndOverlap);
	}

	if (pickup_widget)
	{
		pickup_widget->SetVisibility(false);
	}
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableBase::Interact_Implementation(class UInventoryComponent* inv_comp)
{
	return;
}

FString AInteractableBase::GetInteractText() const
{
	return FString::Printf(TEXT("%s: Press F to %s"), *name, *action);
}

void AInteractableBase::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult
)
{
	ACombatCharacter* combat_character = Cast<ACombatCharacter>(OtherActor);
	if (combat_character)
	{
		if (pickup_widget)
		{
			pickup_widget->SetVisibility(true);
		}
		combat_character->inventory_component->current_interactable = this;
	}
}


void AInteractableBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACombatCharacter* combat_character = Cast<ACombatCharacter>(OtherActor);
	if (combat_character)
	{
		if (pickup_widget)
		{
			pickup_widget->SetVisibility(false);
		}
		combat_character->inventory_component->current_interactable = nullptr;
	}
}

