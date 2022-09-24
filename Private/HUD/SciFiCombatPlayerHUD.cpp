// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SciFiCombatPlayerHUD.h"
#include "GameFramework/PlayerController.h"
#include "SciFiCombat/Public/HUD/PlayerStatusOverlay.h"
#include "SciFiCombat/Public/HUD/WaitingGameWidget.h"
#include "Chat/UserChatWidget.h"

void ASciFiCombatPlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D view_size;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(view_size);
		const FVector2D view_center(view_size.X / 2.f, view_size.Y / 2.f);

		float dynamic_scale = dynamic_max * hud_pack.crosshair_dynamic_movement;

		if (hud_pack.crosshair_texture)
		{
			FVector2D _dynamic(dynamic_scale, dynamic_scale); // 이걸 이용해서 크기를 바꿈
			DrawCrosshair(hud_pack.crosshair_texture, view_center, _dynamic, hud_pack.crosshair_color);
		}
	}
}

void ASciFiCombatPlayerHUD::DrawCrosshair(UTexture2D* texture, FVector2D view_center,
	FVector2D dynamic, FLinearColor color)
{
	// dynamic 파라미터를 통해서 동적으로 width, height를 변경함
	const float width = (texture->GetSizeX() / texture_size_offset) + dynamic.X;
	const float height = (texture->GetSizeY() / texture_size_offset) + dynamic.Y;
	const FVector2D draw_point(
		view_center.X - (width / 2.f),
		view_center.Y - (height / 2.f)
	);

	DrawTexture(
		texture,
		draw_point.X,
		draw_point.Y,
		width,
		height,
		0.f,
		0.f,
		1.f,
		1.f,
		color
	);
}

void ASciFiCombatPlayerHUD::AddWaitingWidget()
{
	APlayerController* player_controller = GetOwningPlayerController();
	if (player_controller && waiting_widget_class)
	{
		waiting_widget = CreateWidget<UWaitingGameWidget>(player_controller, waiting_widget_class);
		waiting_widget->AddToViewport();
	}
}

void ASciFiCombatPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	//InitializeChatWidget();
//	AddStatusOverlay();
}

void ASciFiCombatPlayerHUD::AddStatusOverlay()
{
	APlayerController* player_controller = GetOwningPlayerController();
	if (player_controller && StatusOverlayClass)
	{
		status_overlay = CreateWidget<UPlayerStatusOverlay>(player_controller, StatusOverlayClass);
		status_overlay->AddToViewport();
	}
}

void ASciFiCombatPlayerHUD::InitializeChatWidget(APlayerController* player_controller)
{
	chat_widget = CreateWidget<UUserChatWidget>(player_controller, UUserWidget::StaticClass());
	chat_widget->AddToViewport();
}

void ASciFiCombatPlayerHUD::AddMessage(FName sender, FText message)
{
	chat_widget->AddMessage(sender, message);
}


