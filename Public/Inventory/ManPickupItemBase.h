// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InteractableBase.h"
#include "ManPickupItemBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API AManPickupItemBase : public AInteractableBase
{
	GENERATED_BODY()
public:
	AManPickupItemBase();

	void Interact_Implementation(class UInventoryComponent* inv_comp) override;

protected:
	//UPROPERTY(EditAnywhere)
	//class UStaticMeshComponent* pickup_mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName item_id;
};
