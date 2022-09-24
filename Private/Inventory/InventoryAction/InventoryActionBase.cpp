// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryAction/InventoryActionBase.h"

// Add default functionality here for any IInventoryActionBase functions that are not pure virtual.

IInventoryActionBase::IInventoryActionBase()
{

}

void IInventoryActionBase::UseItemNative()
{
	UE_LOG(LogTemp, Warning, TEXT("Use Item"));
}

void IInventoryActionBase::UseItem()
{
	UseItemNative();
	//UseItemBPImplement();
}