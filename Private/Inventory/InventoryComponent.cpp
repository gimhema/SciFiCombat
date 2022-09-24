// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Character/CombatCharacter.h"
#include "GameState/SciFiCombatGameState.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Inventory/InventoryAction/InventoryActionBase.h"
#include "Inventory/InventoryAction/ItemActionComponent.h"
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
	DOREPLIFETIME(UInventoryComponent, selected_item);
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
			if (!ConsumableCheck(*ItemToAdd))
			{
				// 소비재 아이템이 아닌 경우
				//FInventoryItem ItemStruct;
				//ItemStruct = *ItemToAdd;
				//ServerAddItem(ItemStruct);
				inventory.Add(*ItemToAdd);
				inventory_owner->ReloadInventory();
			}
			// 소비재 아이템의 경우 존재하는 아이템에 Quantity를 증가시키고 함수를 종료한다.
			return true;
		}
	}
	return false;
}

void UInventoryComponent::ServerAddItem_Implementation(const FInventoryItem& ItemToAdd)
{
	NetMulticastAddItem(ItemToAdd);
}
void UInventoryComponent::NetMulticastAddItem_Implementation(const FInventoryItem& ItemToAdd)
{
		inventory.Add(ItemToAdd);
		inventory_owner->ReloadInventory();
}

bool UInventoryComponent::ConsumableCheck(FInventoryItem ItemToAdd)
{
	if (ItemToAdd.IsConsumable)
	{
		// 소비아이템일경우
		for (int32 Index = 0; Index != inventory.Num(); ++Index)
		{
			if (inventory[Index].ItemID == ItemToAdd.ItemID)
			{
				// 아이템을 이미 가지고 있는 경우
				inventory[Index].Quantity++;
				return true;
			}
		}
		// 소비 아이템이지만 아이템을 가지고 있지 않은경우
		return false;
	}
	// 소비 아이템이 아닐 경우
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

void UInventoryComponent::SelectItem(int idx)
{
	is_item_select = true;
	selected_item = inventory[idx];
}

void UInventoryComponent::SelectItemReset()
{
	is_item_select = false;
}

void UInventoryComponent::UseItem()
{
	if (is_item_select)
	{
		//selected_item.InventoryAction.GetInterface()->UseItem();
		//selected_item.UseItem();
		inventory_owner->item_action_component->SetSelectedItem(selected_item);
		inventory_owner->item_action_component->ExecuteItemAction(selected_item.ItemAction);
		// 장착형
		// 소비형
		// 소환형
	}
}

int32 UInventoryComponent::GetItemQuentity(FText item_name)
{
	for (int i = 0; i < inventory.Num(); i++)
	{
		if (inventory[i].Name.ToString() == item_name.ToString())
		{
			return inventory[i].Quantity;
		}
	}

	return 0;
}