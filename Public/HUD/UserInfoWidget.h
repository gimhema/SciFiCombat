// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UUserInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InfoText;

	void SetInfoText(FString text);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* pawn);

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld);
};
