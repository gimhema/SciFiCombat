// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UPlayerStatusOverlay : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
	class UTexture2D* main_skill_texture;

	UPROPERTY(BlueprintReadWrite)
	class UTexture2D* sub_skill_texture;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UImage* main_skill_image;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UImage* sub_skill_image;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UImage* health_bar;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UTextBlock* health_text;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UImage* smash_power_bar;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UTextBlock* smash_power_text;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UImage* mana_bar;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UTextBlock* mana_text;

	//UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	//class UImage* mana_bar;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UImage* weapon_image;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere, BlueprintReadWrite)
	class UTextBlock* ability_alarm_text;


	UPROPERTY(meta = (BindWidget), VisibleAnyWhere)
	UTextBlock* score_text;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere)
	UTextBlock* score_amount;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere)
	UTextBlock* death_text;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere)
	UTextBlock* death_amount;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere)
	UTextBlock* ammo_amount;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere)
	UTextBlock* carried_ammo_amount;

	UPROPERTY(meta = (BindWidget), VisibleAnyWhere)
	UTextBlock* match_countdown_text;


	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	float health_percent = 1.f;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	float smash_power_percent = 1.f;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	float mana_percent = 1.f;
};
