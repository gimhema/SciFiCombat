// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryAction/ItemActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Character/CombatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Inventory/InventoryComponent.h"

// Sets default values for this component's properties
UItemActionComponent::UItemActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UItemActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemActionComponent, selected_item);
	//DOREPLIFETIME(UItemActionComponent, selected_recipe);
	DOREPLIFETIME(UItemActionComponent, equip_item_class);
	DOREPLIFETIME(UItemActionComponent, craft_item_class);
	DOREPLIFETIME(UItemActionComponent, spawn_item_class);
	DOREPLIFETIME(UItemActionComponent, attach_item_socket);
	DOREPLIFETIME(UItemActionComponent, equip_item_socket_name);

	DOREPLIFETIME(UItemActionComponent, selected_ingredient_idxs);
	DOREPLIFETIME(UItemActionComponent, selected_ingredient_qauntities);
}


// Called every frame
void UItemActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemActionComponent::InitializeItemActionComponent(class ACombatCharacter* _inventory_owner)
{
	inventory_owner = _inventory_owner;
	InitializeItemActionDelegates();
}

void UItemActionComponent::InitializeItemActionDelegates()
{
	item_action_delegate_map.Add("SpawnObj", CreateItemActionDelegate("ItemAction_SpawnObject"));
	item_action_delegate_map.Add("AttachMesh", CreateItemActionDelegate("ItemAction_AttachMesh"));
	item_action_delegate_map.Add("Consume", CreateItemActionDelegate("ItemAction_Consume"));
}

FItemActionDelegate UItemActionComponent::CreateItemActionDelegate(FString skill_name)
{
	FItemActionDelegate item_action_delegate;
	item_action_delegate.BindUFunction(this, FName(skill_name));
	return item_action_delegate;
}

void UItemActionComponent::ItemAction_SpawnObject()
{
	// selected_item을 활용하여 오브젝트 스폰
	UE_LOG(LogTemp, Warning, TEXT("Spawn Action"));
	APawn* instigator = Cast<APawn>(GetOwner());

	//FActorSpawnParameters spawn_params;
	//spawn_params.Owner = inventory_owner;
	//spawn_params.Instigator = inventory_owner;

	UWorld* world = GetWorld();
	FVector spawn_loc = inventory_owner->ability_object_muzzle->GetComponentLocation();
	FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);
	if (world)
	{
		 world->SpawnActor<AActor>(
			spawn_item_class,
			spawn_loc,
			spawn_rot
			);
	}
}

void UItemActionComponent::ItemAction_AttachMesh()
{
	// selected_item을 활용하여 메시 어태치
	UE_LOG(LogTemp, Warning, TEXT("Attach Mesh Action"));
	APawn* instigator = Cast<APawn>(GetOwner());

	//FActorSpawnParameters spawn_params;
	//spawn_params.Owner = inventory_owner;
	//spawn_params.Instigator = inventory_owner;
	//const USkeletalMeshSocket* attach_socket = inventory_owner->GetMesh()->GetSocketByName(equip_item_socket_name);

	AActor* attach_obj;

	UWorld* world = GetWorld();
	if (attach_item_socket)
	{
		FTransform socket_transform = attach_item_socket->GetSocketTransform(inventory_owner->GetMesh());
		FVector spawn_loc = socket_transform.GetLocation();
		FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);
		if (world)
		{
			attach_obj = world->SpawnActor<AActor>(
				equip_item_class,
				spawn_loc,
				spawn_rot
				);
			//attach_socket->AttachActor(attach_obj, inventory_owner->GetMesh());
			//attach_obj->AttachToActor(inventory_owner, FAttachmentTransformRules::KeepWorldTransform, equip_item_socket_name);
			attach_item_socket->AttachActor(attach_obj, inventory_owner->GetMesh());
		}
	}
}

void UItemActionComponent::ItemAction_Consume()
{
	// selected_item을 활용하여 특정 값을 소비
	UE_LOG(LogTemp, Warning, TEXT("Consume Action"));
	
	// 일단 테스트를 위해 체력만
	inventory_owner->current_health = UpdateConsume(
		selected_item.is_add, 
		inventory_owner->current_health, 
		selected_item.CosnumeValue);
}

void UItemActionComponent::SetSelectedItem(const FInventoryItem& item_info)
{
	ServerSetSelectedItem(item_info);
}

void UItemActionComponent::ServerSetSelectedItem_Implementation(const FInventoryItem& item_info)
{
	MulticastSelectedItem(item_info);
}

void UItemActionComponent::MulticastSelectedItem_Implementation(const FInventoryItem& item_info)
{
	selected_item = item_info;
	equip_item_class = selected_item.AttachObjClass;
	spawn_item_class = selected_item.SpawnObjClass;
	equip_item_socket_name = FName(selected_item.ItemSocketName);
	attach_item_socket = inventory_owner->GetMesh()->GetSocketByName(equip_item_socket_name);
}

float UItemActionComponent::UpdateConsume(bool add_opt, float target_value, float add_value)
{
	if (add_opt) { return target_value + add_value; }
	return target_value - add_value;

}

void UItemActionComponent::ExecuteItemAction(const FString& action_name)
{
	ServerExecuteItemAction(action_name);
}

void UItemActionComponent::ServerExecuteItemAction_Implementation(const FString& action_name)
{
	MulticastExecuteItemAction(action_name);
}

void UItemActionComponent::MulticastExecuteItemAction_Implementation(const FString& action_name)
{
	if (item_action_delegate_map[action_name].IsBound())
	{
		item_action_delegate_map[action_name].Execute();
	}
	//CallItemAction(action_name);
}

void UItemActionComponent::CallItemAction(const FString& action_name)
{
	//if (!inventory_owner->HasAuthority()) return;

	if (item_action_delegate_map[action_name].IsBound())
	{
		item_action_delegate_map[action_name].Execute();
	}
}

void UItemActionComponent::SelectRecipe(const FItemCraftRecipe& recipe_info)
{
	selected_recipe = recipe_info;
	ServerSelectRecipe(recipe_info.target_item);
}

void UItemActionComponent::ServerSelectRecipe_Implementation(TSubclassOf<AActor> item_class)
{
	MulticastSelectRecipe(item_class);
}

void UItemActionComponent::MulticastSelectRecipe_Implementation(TSubclassOf<AActor> item_class)
{
	craft_item_class = item_class;
}

// Recipe 슬롯 클릭 시
// 슬롯에 할당된 인덱스로 우측의 . . 재료 리스트들을 나열
// SelectRecipe 함수 실행하여 . . .

// 크래프트 버튼 클릭 시
// 1. CheckItemCraftCondition() 제작 가능여부 판단
// 2. 1번에서 트루가 리턴될 경우 ManufactureIngredient() 아이템으로 재료들 소진
// 3. CraftItem() 함수 실행


bool UItemActionComponent::CheckItemCraftCondition()
{
	int check_count = 0;

	for (auto& elem : selected_recipe.needs)
	{
		for (int i = 0; i < inventory_owner->inventory_component->inventory.Num(); i++)
		{
			if (inventory_owner->inventory_component->inventory[i].Name.ToString() == elem.Key.ToString())
			{
				if (inventory_owner->inventory_component->inventory[i].Quantity >= elem.Value)
				{
					check_count++;
					IngredientUpdate(i, elem.Value);
					//selected_ingredient_idxs.Add(i);
					//selected_ingredient_qauntities.Add(elem.Value);
				}
			}
		}
	}
	if (check_count == selected_recipe.needs.Num())
	{
		return true;
	}
	else
	{
		IngredientReset();
		//selected_ingredient_idxs.Empty();
		//selected_ingredient_qauntities.Empty();
	}
	return false;
}

void UItemActionComponent::IngredientUpdate(int32 ing_idx, int32 ing_quanty)
{
	ServerIngredientUpdate(ing_idx, ing_quanty);
}

void UItemActionComponent::ServerIngredientUpdate_Implementation(int32 ing_idx, int32 ing_quanty)
{
	MulticastIngredientUpdate(ing_idx, ing_quanty);
}

void UItemActionComponent::MulticastIngredientUpdate_Implementation(int32 ing_idx, int32 ing_quanty)
{
	if (inventory_owner->HasAuthority())
	{
		selected_ingredient_idxs.Add(ing_idx);
		selected_ingredient_qauntities.Add(ing_quanty);
	}
}

void UItemActionComponent::IngredientReset()
{
	selected_ingredient_idxs.Empty();
	selected_ingredient_qauntities.Empty();
	//ServerIngredientReset();
}

void UItemActionComponent::ServerIngredientReset_Implementation()
{
	MulticastIngredientReset();
}

void UItemActionComponent::MulticastIngredientReset_Implementation()
{
		selected_ingredient_idxs.Empty();
		selected_ingredient_qauntities.Empty();
}

void UItemActionComponent::CraftItem(const FVector& create_loc)
{
	ServerCraftItem(create_loc);
}

void UItemActionComponent::ServerCraftItem_Implementation(const FVector& create_loc)
{
	MulticastCraftItem(create_loc);
}

void UItemActionComponent::MulticastCraftItem_Implementation(const FVector& create_loc)
{
	if (inventory_owner->HasAuthority())
	{
		UWorld* world = GetWorld();
		FRotator spawn_rot = FRotator(0.f, 0.f, 0.f);
		if (world)
		{
			world->SpawnActor<AActor>(
				craft_item_class,
				create_loc,
				spawn_rot
				);
		}
	}
}

void UItemActionComponent::ManufactureIngredient()
{
	for (auto& Elem : selected_recipe.needs)
	{
		for (int i = 0; i < inventory_owner->inventory_component->inventory.Num(); i++)
		{
			if (inventory_owner->inventory_component->inventory[i].Name.ToString() == Elem.Key.ToString())
			{
				int32 need_qanty = Elem.Value;
				int32 result = inventory_owner->inventory_component->inventory[i].Quantity - need_qanty;
				inventory_owner->inventory_component->inventory[i].Quantity = result;
			}
		}
	}
	for (int i = 0; i < inventory_owner->inventory_component->inventory.Num(); i++)
	{
		if (inventory_owner->inventory_component->inventory[i].Quantity <= 0)
		{
			inventory_owner->inventory_component->inventory.RemoveAt(i);
		}
	}
	/*for (int i = 0; i < selected_ingredient_idxs.Num(); i++)
	{
		UseIngredient(selected_ingredient_qauntities[i], selected_ingredient_idxs[i]);
	}

	IngredientReset();*/
	//selected_ingredient_idxs.Empty();
	//selected_ingredient_qauntities.Empty();
}

void UItemActionComponent::UseIngredient(int32 need_quantity, int32 inventory_idx)
{
	int32 result = inventory_owner->inventory_component->inventory[inventory_idx].Quantity - need_quantity;

	if (result <= 0)
	{
		inventory_owner->inventory_component->inventory.RemoveAt(inventory_idx);
	}
	else
	{
		inventory_owner->inventory_component->inventory[inventory_idx].Quantity = result;
	}
	//ServerUseIngredient(need_quantity, inventory_idx);
}

void UItemActionComponent::ServerUseIngredient_Implementation(int32 need_quantity, int32 inventory_idx)
{
	MulticastUseIngredient(need_quantity, inventory_idx);
}

void UItemActionComponent::MulticastUseIngredient_Implementation(int32 need_quantity, int32 inventory_idx)
{
		int32 result = inventory_owner->inventory_component->inventory[inventory_idx].Quantity - need_quantity;

		if (result <= 0)
		{
			inventory_owner->inventory_component->inventory.RemoveAt(inventory_idx);
		}
		else
		{
			inventory_owner->inventory_component->inventory[inventory_idx].Quantity = result;
		}
}




