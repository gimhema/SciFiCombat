// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SciFiCombatPlayerHUD.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* crosshair_texture;
	float crosshair_dynamic_movement;
	float crosshair_dynamic_rotation;
	FLinearColor crosshair_color;
};

UCLASS()
class SCIFICOMBAT_API ASciFiCombatPlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;



	UPROPERTY(EditAnywhere, Category = "Status")
	TSubclassOf<class UUserWidget> StatusOverlayClass;
	void AddStatusOverlay();

	UPROPERTY()
	class UPlayerStatusOverlay* status_overlay;

	UPROPERTY(EditAnywhere, Category = "Waiting")
	TSubclassOf<UUserWidget> waiting_widget_class;

	UPROPERTY()
	class UWaitingGameWidget* waiting_widget;

	void AddWaitingWidget();

protected:
	virtual void BeginPlay() override;


private:
	FHUDPackage hud_pack;

	void DrawCrosshair(UTexture2D* texture, FVector2D view_center, FVector2D dynamic,
		FLinearColor color);

	

	UPROPERTY(EditAnywhere)
	float dynamic_max = 16.f; // Default 16.f

	UPROPERTY(EditAnywhere)
	float dynamic_rotaiton_max = 16.f; // Default 16.f

public:
	FORCEINLINE void SetHUDPack(const FHUDPackage& pack) { hud_pack = pack; }

public:
	float texture_size_offset = 1.f;
	FORCEINLINE void SetCrosshairTextureSizeOffset(float offset) { texture_size_offset = offset; }

public:
	// Chat System Member
	UPROPERTY(BlueprintReadWrite)
	class UUserChatWidget* chat_widget;

public:
	// Chat System Method
	UFUNCTION(BlueprintCallable)
	void InitializeChatWidget(APlayerController* player_controller);
	UFUNCTION(BlueprintCallable)
	void AddMessage(FName sender, FText message);
};
