// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UserInfoWidget.h"
#include "Components/TextBlock.h"

void UUserInfoWidget::ShowPlayerNetRole(APawn* pawn)
{
	ENetRole RemoteRole = pawn->GetRemoteRole();
	// ENetRole LocalRole = pawn->GetLocalRole();
	FString Role;
	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	}
	FString RemoteRoleString = FString::Printf(TEXT("Remote Role : %s"), *Role);
	SetInfoText(RemoteRoleString);
}

void UUserInfoWidget::SetInfoText(FString text)
{
	if (InfoText)
	{
		InfoText->SetText(FText::FromString(text));
	}
}

void UUserInfoWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}



