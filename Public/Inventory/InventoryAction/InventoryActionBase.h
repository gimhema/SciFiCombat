// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryActionBase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryActionBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SCIFICOMBAT_API IInventoryActionBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//UPROPERTY()
	//class ACombatCharacter* inventory_owner;

public:
	IInventoryActionBase();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UseItemBPImplement();
	
	virtual void UseItemNative();


	UFUNCTION()
	virtual void UseItem();
};
