// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SciFiCombat/Public/PlayerController/SciFiCombatPlayerController.h"
#include "CharacterClassSpawner.generated.h"

UCLASS()
class SCIFICOMBAT_API ACharacterClassSpawner : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterClassSpawner();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	// Replicate
	UPROPERTY(Replicated)
	class ACombatMasterBase* combat_master;

	UPROPERTY(Replicated)
	class AMagicianBase* magician;

	UPROPERTY(Replicated)
	class AGuardianBase* guardian;

	//UPROPERTY(Replicated)
	//ECombatCharacterClass selected_class;
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
	UFUNCTION()
	void SummonCharacter();
	UFUNCTION()
	void SpawnCharacter();
};
