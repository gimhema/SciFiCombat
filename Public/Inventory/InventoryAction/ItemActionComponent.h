// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InteractableBase.h"
#include "Inventory/InventoryItem.h"
#include "Inventory/ItemCraftRecipe.h"
#include "ItemActionComponent.generated.h"

DECLARE_DELEGATE(FItemActionDelegate)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCIFICOMBAT_API UItemActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY()
	class ACombatCharacter* inventory_owner;

	UPROPERTY(Replicated, BlueprintReadWrite)
	FInventoryItem selected_item;

	// Equip Item
	UPROPERTY(Replicated, BlueprintReadWrite)
	TSubclassOf<AActor> equip_item_class;
	UPROPERTY(Replicated, BlueprintReadWrite)
	FName equip_item_socket_name;
	UPROPERTY(Replicated, BlueprintReadWrite)
	const USkeletalMeshSocket* attach_item_socket;

	// Spawn Item
	UPROPERTY(Replicated, BlueprintReadWrite)
	TSubclassOf<AActor> spawn_item_class;

	void SetSelectedItem(const FInventoryItem& item_info);
	UFUNCTION(Server, Reliable)
	void ServerSetSelectedItem(const FInventoryItem& item_info);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSelectedItem(const FInventoryItem& item_info);

	TMap<FString, FItemActionDelegate> item_action_delegate_map;

	UPROPERTY(BlueprintReadWrite)
	FItemCraftRecipe selected_recipe;
	UPROPERTY(Replicated, BlueprintReadWrite)
	TSubclassOf<AActor> craft_item_class;

	UPROPERTY(Replicated)
	TArray<int32> selected_ingredient_idxs;
	UPROPERTY(Replicated)
	TArray<int32> selected_ingredient_qauntities;

	UFUNCTION()
	void IngredientUpdate(int32 ing_idx, int32 ing_quanty);
	UFUNCTION(Server, Reliable)
	void ServerIngredientUpdate(int32 ing_idx, int32 ing_quanty);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastIngredientUpdate(int32 ing_idx, int32 ing_quanty);

	UFUNCTION()
	void IngredientReset();
	UFUNCTION(Server, Reliable)
	void ServerIngredientReset();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastIngredientReset();

public:

	void InitializeItemActionComponent(class ACombatCharacter* _inventory_owner);

	FItemActionDelegate CreateItemActionDelegate(FString action_name);
	
	void InitializeItemActionDelegates();

	UFUNCTION()
	void ExecuteItemAction(const FString& action_name);
	UFUNCTION(Server, Reliable)
	void ServerExecuteItemAction(const FString& action_name);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastExecuteItemAction(const FString& action_name);
	UFUNCTION()
	void CallItemAction(const FString& action_name);
public:
	// Item Actions
	UFUNCTION()
	void ItemAction_SpawnObject();
	UFUNCTION()
	void ItemAction_AttachMesh();
	UFUNCTION()
	void ItemAction_Consume();
	float UpdateConsume(bool add_opt, float target_value, float add_value);

public:
	// Item Craft
	UFUNCTION(BlueprintCallable)
	void SelectRecipe(const FItemCraftRecipe& recipe_info);
	UFUNCTION(Server, Reliable)
	void ServerSelectRecipe(TSubclassOf<AActor> item_class);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSelectRecipe(TSubclassOf<AActor> item_class);

	UFUNCTION(BlueprintCallable)
	bool CheckItemCraftCondition();

	UFUNCTION(BlueprintCallable)
	void ManufactureIngredient();

	UFUNCTION(BlueprintCallable)
	void UseIngredient(int32 need_quantity, int32 inventory_idx);
	UFUNCTION(Server, Reliable)
	void ServerUseIngredient(int32 need_quantity, int32 inventory_idx);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUseIngredient(int32 need_quantity, int32 inventory_idx);

	UFUNCTION(BlueprintCallable)
	void CraftItem(const FVector& create_loc);
	UFUNCTION(Server, Reliable)
	void ServerCraftItem(const FVector& create_loc);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastCraftItem(const FVector& create_loc);
};
