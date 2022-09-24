// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Inventory/InventoryAction/InventoryActionBase.h"
#include "ItemCraftRecipe.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemCraftRecipe : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemCraftRecipe();

	UPROPERTY(Category = RecipeInfo, EditAnywhere, BlueprintReadWrite)
	int32 RecipeID;

	UPROPERTY(Category = RecipeInfo, EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> needs;

	UPROPERTY(Category = RecipeInfo, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> target_item;

	UPROPERTY(Category = RecipeInfo, EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(Category = RecipeInfo, EditAnywhere, BlueprintReadWrite)
	FText Name;
};

