// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ReturnHome.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "../Plugins/MultiplayerSessions/Source/MultiplayerSessions/Public/MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"

void UReturnHome::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true; 

	UWorld* world = GetWorld();
	if (world)
	{
		player_controller = player_controller == nullptr ? world->GetFirstPlayerController() : player_controller;
		if (player_controller)
		{
			FInputModeGameAndUI input_mode_data;
			input_mode_data.SetWidgetToFocus(TakeWidget());
			player_controller->SetInputMode(input_mode_data);
			player_controller->SetShowMouseCursor(true);
		}
	}
	if (return_button && !return_button->OnClicked.IsBound())
	{
		return_button->OnClicked.AddDynamic(this, &UReturnHome::OnClickedReturnButton);
	}
	UGameInstance* game_instance = GetGameInstance();
	if (game_instance)
	{
		multiplayerSessionSubsystem = game_instance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (multiplayerSessionSubsystem)
		{
			multiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(
				this,
				&UReturnHome::OnDestroySession
			);
		}
	}


}

bool UReturnHome::Initialize()
{
	if (!Super::Initialize()){ return false; }
	


	return true;
}

void UReturnHome::MenuTearDown()
{
	RemoveFromParent();
	UWorld* world = GetWorld();
	if (world)
	{
		player_controller = player_controller == nullptr ? world->GetFirstPlayerController() : player_controller;
		if (player_controller)
		{
			FInputModeGameOnly input_mode_data;
			player_controller->SetInputMode(input_mode_data);
			player_controller->SetShowMouseCursor(false);
		}
	}
	if (return_button && return_button->OnClicked.IsBound())
	{
		return_button->OnClicked.RemoveDynamic(this, &UReturnHome::OnClickedReturnButton);
	}
	if (multiplayerSessionSubsystem && multiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		multiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(
			this,
			&UReturnHome::OnDestroySession
		);
	}

}

void UReturnHome::OnDestroySession(bool isSuccess)
{
	if(!isSuccess)
	{
		return_button->SetIsEnabled(true);
		return;
	}
	UWorld* world = GetWorld();
	if (world)
	{
		AGameModeBase* game_mode = world->GetAuthGameMode<AGameModeBase>();
		if (game_mode)
		{
			game_mode->ReturnToMainMenuHost();
		}
		else
		{
			player_controller = player_controller == nullptr ? world->GetFirstPlayerController() : player_controller;
			if (player_controller)
			{
				player_controller->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UReturnHome::OnClickedReturnButton()
{
	return_button->SetIsEnabled(false);

	if (multiplayerSessionSubsystem)
	{
		multiplayerSessionSubsystem->DestroySession();
	}
}



