// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubSkillSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API USubSkillSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	UPROPERTY()
	class ASciFiCombatPlayerController* player_combat_controller;

	UPROPERTY(BlueprintReadWrite)
	class UTexture2D* main_skill_texture;

	UPROPERTY(meta = (BindWidget))
	class UButton* subskill_select_blackhole;
	UPROPERTY(meta = (BindWidget))
	class UButton* subskill_select_impulse;
	UPROPERTY(meta = (BindWidget))
	class UButton* subskill_select_booster;
	UPROPERTY(meta = (BindWidget))
	class UButton* subskill_select_jumpup;
	UPROPERTY(meta = (BindWidget))
	class UButton* subskill_select_heal;

	UFUNCTION()
	void OnClickedSubskillSelectBlackHole();
	UFUNCTION(Server, Reliable)
	void ServerSubskillSelectBlackHole();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSubskillSelectBlackHole();

	UFUNCTION()
	void OnClickedSubskillSelectImpulse();
	UFUNCTION(Server, Reliable)
	void ServerSubskillSelectImpulse();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSubskillSelectImpulse();

	UFUNCTION()
	void OnClickedSubskillSelectBooster();
	UFUNCTION(Server, Reliable)
	void ServerSubskillSelectBooster();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSubskillSelectBooster();

	UFUNCTION()
	void OnClickedSubskillSelectJumpup();
	UFUNCTION(Server, Reliable)
	void ServerSubskillSelectJumpup();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSubskillSelectJumpup();

	UFUNCTION()
	void OnClickedSubskillSelectHeal();
	UFUNCTION(Server, Reliable)
	void ServerSubskillSelectHeal();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSubskillSelectHeal();


};
