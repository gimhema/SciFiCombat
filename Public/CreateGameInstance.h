// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "CreateGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UCreateGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UCreateGameInstance();
	virtual void Init() override;

protected:

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	IOnlineSessionPtr SessionInterface;



	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
		void CreateServer();

	UFUNCTION(BlueprintCallable)
		void JoinServer();
};
