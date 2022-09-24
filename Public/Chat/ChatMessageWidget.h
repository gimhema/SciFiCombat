// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = HUD, BlueprintReadWrite)
	FName sender;
	UPROPERTY(EditAnywhere, Category = HUD, BlueprintReadWrite)
	FText message;
	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UTextBlock* sender_text;
	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UTextBlock* message_text;
};
