// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "GrappleCableActor.h"
#include "GrappleComponent.generated.h"

UENUM(BlueprintType)
enum class EGrappleState : uint8
{
	EGS_ReadyToFire UMETA(DisplayName = "Ready To Fire"),
	EGS_Firing UMETA(DisplayName = "Firing"),
	EGS_AttachToTarget UMETA(DisplayName = "Attach To Target")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCIFICOMBAT_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrappleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void GrappleStateLoop();
	UFUNCTION()
	bool isInUse();
	UFUNCTION()
	void FireGrapple(FVector target_location, FVector local_offset);
	UFUNCTION()
	void CancelGrapple();

	UFUNCTION(Server, Reliable)
	void ServerFireGrapple(FVector target_location, FVector local_offset);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastFireGrapple(FVector target_location, FVector local_offset);

	UFUNCTION(Server, Reliable)
	void ServerCancelGrapple();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastCancelGrapple();

	FORCEINLINE ACombatCharacter* GetGrappleOwner() const { return grapple_owner; }
	void SetGrappleOwner(ACombatCharacter* _owner) { grapple_owner = _owner; }

	//
	UFUNCTION()
	FVector CableStartLocation(FVector direction);
	UFUNCTION()
	FVector ToGrappleHook();
	UFUNCTION()
	FVector ToGrappleHook2D();

public:
	UPROPERTY()
	ACombatCharacter* grapple_owner;

	UPROPERTY(Replicated)
	EGrappleState grapple_state = EGrappleState::EGS_ReadyToFire;

	UPROPERTY(EditAnywhere)
	float speed = 5000.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGrappleHook> grapple_hook_class;
	UPROPERTY(Replicated)
	class AGrappleHook* grapple_hook;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGrappleCableActor> grapple_cable_actor_class;
	UPROPERTY(Replicated)
	AGrappleCableActor* grapple_cable_actor;

	UPROPERTY(EditAnywhere)
	FVector InitiailHookDirection2D;
};
