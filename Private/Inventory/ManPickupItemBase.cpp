// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ManPickupItemBase.h"
#include "Inventory/InventoryComponent.h"



AManPickupItemBase::AManPickupItemBase()
{
	//pickup_mesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	//pickup_mesh->SetupAttachment(RootComponent);
	//RootComponent = Cast<USceneComponent>(pickup_mesh);

	item_id = FName("No ID");

	Super::name = "Item";
	Super::action = "pickup";
}

void AManPickupItemBase::Interact_Implementation(class UInventoryComponent* inv_comp)
{
	Super::Interact_Implementation(inv_comp);

	//AInventoryController* IController = Cast<AInventoryController>(Controller);
	if (inv_comp->AddItemToInventoryByID(item_id))
	{
		Destroy();
	}
}