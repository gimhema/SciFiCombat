// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnHome.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UReturnHome : public UUserWidget
{
	GENERATED_BODY()
public:
	void MenuSetup();
	void MenuTearDown();

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool isSuccess);
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* return_button;

	UFUNCTION()
	void OnClickedReturnButton();

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* multiplayerSessionSubsystem;
	
	UPROPERTY()
	class APlayerController* player_controller;
};
