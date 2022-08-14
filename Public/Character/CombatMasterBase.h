// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CombatCharacter.h"
#include "CombatMasterBase.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API ACombatMasterBase : public ACombatCharacter
{
	GENERATED_BODY()
public:
	ACombatMasterBase();
	virtual void PressAbility1() override;
	virtual void PressAbility2() override;
	virtual void PressAbility3() override;
public:
	UFUNCTION(Server, Reliable)
	void ServerUseAbility1();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUseAbility1();

	UFUNCTION(Server, Reliable)
	void ServerUseAbility2();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUseAbility2();

	UFUNCTION(Server, Reliable)
	void ServerUseAbility3();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUseAbility3();
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAbilityObjectBase> ability1_object;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class AAbilityObjectBase> ability2_object;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATeleportGrenadeObject> ability2_object;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAbilityObjectBase> ability3_object;
};
