// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

UCLASS()
class SCIFICOMBAT_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* pickup_mesh;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* item_collection_sphere;

	UFUNCTION(BlueprintNativeEvent)
	void Interact(class UInventoryComponent* inv_comp);
	virtual void Interact_Implementation(class UInventoryComponent* inv_comp);

	UPROPERTY(EditDefaultsOnly)
	FString name;

	UPROPERTY(EditDefaultsOnly)
	FString action;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	FString GetInteractText() const;

	UPROPERTY(VisibleAnyWhere, Category = "Weapon Property")
	class UWidgetComponent* pickup_widget;

	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool isFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
