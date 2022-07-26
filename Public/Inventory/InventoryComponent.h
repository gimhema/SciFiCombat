// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InteractableBase.h"
#include "Inventory/InventoryItem.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCIFICOMBAT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();



	UFUNCTION(BlueprintCallable, Category = "Utils")
	int32 GetInventoryWeight();

	UFUNCTION(BlueprintCallable, Category = "Utils")
	bool AddItemToInventoryByID(FName ID);

	UFUNCTION(Server, Reliable)
	void ServerAddItem(const FInventoryItem & ItemToAdd);
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastAddItem(const FInventoryItem & ItemToAdd);


	/** Function to check for the closest Interactable in sight and in range. */
	void CheckForInteractables();

	void Interaction();
	void InteractPickupItem();
	UFUNCTION(Server, Reliable)
	void ServerInteractPickup();
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastInteractPickup();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//void Interact();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	class AInteractableBase* current_interactable;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> inventory;

	UPROPERTY(Replicated, BlueprintReadWrite)
	FInventoryItem selected_item;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 money;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 inventory_slot_limit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 inventory_weight_limit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 interact_range = 1500;

	UPROPERTY()
	class ACombatCharacter* inventory_owner;

	UPROPERTY()
	bool is_item_select = false;
public:

	UFUNCTION()
	void SetInventoryOwer(class ACombatCharacter* _owner);

	UFUNCTION()
	bool ConsumableCheck(FInventoryItem ItemToAdd);

	UFUNCTION(BlueprintCallable)
	int32 GetItemQuentity(FText item_name);


	UFUNCTION(BlueprintCallable)
	void SelectItem(int idx);
	UFUNCTION(BlueprintCallable)
	void SelectItemReset();
	UFUNCTION(BlueprintCallable)
	void UseItem();


};
