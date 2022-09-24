// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UUserChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UScrollBox* messages;
	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UEditableTextBox* message_text_box;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> user_chat_message_subclass;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddMessage(const FName sender, const FText& message);
};
