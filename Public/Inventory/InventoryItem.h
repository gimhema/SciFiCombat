// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Inventory/InventoryAction/InventoryActionBase.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase, public IInventoryActionBase
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	TSubclassOf<IInventoryActionBase> InventoryAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TScriptInterface<IInventoryActionBase> InventoryAction;
	
	//IInventoryActionBase* InventoryAction;
	//virtual void UseItem() const override;

	FInventoryItem();
	//FInventoryItem() : InventoryAction(NULL) {  }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemAction;

	// 소켓 장착용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemSocketName;
	UPROPERTY(EditAnywhere, Category = Awakening)
		TSubclassOf<AActor> AttachObjClass;
	UPROPERTY(EditAnywhere, Category = Awakening)
		bool is_equip = false;
	// 스탯 업
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CosnumeValueName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CosnumeValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool is_add = true;
	// 스폰용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> SpawnObjClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsConsumable = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Quantity = 1;

	bool operator==(const FInventoryItem& OtherItem) const
	{
		if (ItemID == OtherItem.ItemID)
			return true;
		return false;
	}

	//UFUNCTION(BlueprintCallable, Category = "ItemMethod")
	//void ItemMethodTest()
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Item Method Test"));
	//}
};



//class SCIFICOMBAT_API InventoryItem
//{
//public:
//	InventoryItem();
//	~InventoryItem();
//};
