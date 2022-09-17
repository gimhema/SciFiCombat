// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Character/CombatCharacter.h"
#include "GameState/SciFiCombatGameState.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	inventory_slot_limit = 50;
	inventory_weight_limit = 500;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	//inventory_owner = Cast<ACombatCharacter>(GetOwner());
	// ...
	
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, current_interactable);
	DOREPLIFETIME(UInventoryComponent, inventory);
}

void UInventoryComponent::SetInventoryOwer(class ACombatCharacter* _owner)
{
	inventory_owner = _owner;
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//if (inventory_owner)
	//{
	//	CheckForInteractables();
	//}
	// ...
}

int32 UInventoryComponent::GetInventoryWeight()
{
	int32 Weight = 0;
	for (auto& Item : inventory)
	{
		Weight += Item.Weight;
	}

	return Weight;
}
bool UInventoryComponent::AddItemToInventoryByID(FName ID)
{
	ASciFiCombatGameState* GameState = Cast<ASciFiCombatGameState>(GetWorld()->GetGameState());
	UDataTable* ItemTable = GameState->GetItemDB();
	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

	if (ItemToAdd)
	{
		// If a Slot- or WeightLimit are not needed remove them in this line
		if (inventory.Num() < inventory_slot_limit && GetInventoryWeight() + ItemToAdd->Weight <= inventory_weight_limit)
		{
			inventory.Add(*ItemToAdd);
			inventory_owner->ReloadInventory();
			return true;
		}
	}
	return false;
}

/** Function to collect every AutoPickup in range. */
/** Function to check for the closest Interactable in sight and in range. */
void UInventoryComponent::CheckForInteractables()
{

}

void UInventoryComponent::Interaction()
{
	if (current_interactable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get Item"));
		current_interactable->Interact(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Valid Getting Item"));
	}
}

void UInventoryComponent::InteractPickupItem()
{
	ServerInteractPickup();
}

void UInventoryComponent::ServerInteractPickup_Implementation()
{
	NetMulticastInteractPickup();
}
void UInventoryComponent::NetMulticastInteractPickup_Implementation()
{
	Interaction();
}
