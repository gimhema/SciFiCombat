// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UWaitingGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UTextBlock* waiting_time_text;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere)
	UTextBlock* waiting_announcement_text;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere)
	UTextBlock* info_text;

	UPROPERTY(meta = (BindWidget))
	class UButton* select_combat_master;

	UPROPERTY(meta = (BindWidget))
	class UButton* select_magician;

	UPROPERTY(meta = (BindWidget))
	class UButton* select_guardian;

	UPROPERTY()
	class ASciFiCombatPlayerController* player_combat_controller;

	UFUNCTION()
	void OnClickedSelectCombatMaster();
	UFUNCTION(Server, Reliable)
	void ServerSelectCombatMaster();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSelectCombatMaster();
	
	UFUNCTION()
	void OnClickedSelectMagician();
	UFUNCTION(Server, Reliable)
	void ServerSelectMagician();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSelectMagician();

	UFUNCTION()
	void OnClickedSelectGuardian();
	UFUNCTION(Server, Reliable)
	void ServerSelectGuardian();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSelectGuardian();
};
