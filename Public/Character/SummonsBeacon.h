// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SummonsBeacon.generated.h"

UCLASS()
class SCIFICOMBAT_API ASummonsBeacon : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASummonsBeacon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACombatMasterBase> combat_master_class;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMagicianBase> magician_class;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGuardianBase> guardian_class;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACombatCharacter> none_class;

	UPROPERTY(EditAnywhere)
	float spawn_delay = 1.5f;

	UPROPERTY(EditAnywhere)
	float destroy_delay = 3.5f;
		
public:
	UFUNCTION()
	void SpawnSelectCharacter();
	UFUNCTION(Server, Reliable)
	void ServerSpawnSelectCharacter();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSpawnSelectCharacter();
};
