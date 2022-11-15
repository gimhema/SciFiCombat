// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DragonBossAIController.generated.h"



UENUM(BlueprintType)
enum class EDragonStatus : uint8
{
	EDS_Sleep UMETA(DisplayName = "Dragon Sleep"),
	EDS_Fly UMETA(DisplayName = "Dragon Fly"),
	EDS_SpitFire UMETA(DisplayName = "Dragon Spit Fire"),
	EDS_SpreadFire UMETA(DisplayName = "Dragon Spread Fire"),
	EDS_LinearFire UMETA(DisplayName = "Dragon Linear Fire"),
	EDS_MeleePhase UMETA(DisplayName = "Dragon Melee Phase"),
	EDS_RangePhase UMETA(DisplayName = "Dragon Range Phase")
};

UCLASS()
class SCIFICOMBAT_API ADragonBossAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> Waypoints; // BP에서 설정
	UPROPERTY(BlueprintReadWrite)
	int32 current_waypoint_idx = 0;

	class ADragonBossBase* controlled_dragon;

	EDragonStatus dragoun_status = EDragonStatus::EDS_MeleePhase;
	bool fly_lock = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector spread_fire_location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector linear_fire_start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector linear_fire_end;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float follow_distance = 900.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float status_delay = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 spit_fire_count_max = 3;
	int32 spit_fire_count = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 spread_fire_count_max = 7;
	//int32 spread_fire_count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 bite_count_max = 3;
	int32 bite_count = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 linear_fire_count_max = 13;
	//int32 linear_fire_count = 0;

	// Check Distacne
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fire_distance = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float rush_distance = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float trace_distance = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float attack_distance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float rush_speed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float trace_speed = 100.f;

	//bool can_do_action = true;
public:
	ADragonBossAIController();

	void SetDrgonStatus(EDragonStatus _status);

	void CheckDistance();
	void DragonStateProcess();



	void MoveToWaypoint(FVector _Waypoint);
	void TravelWaypoints();
	
	bool CheckWaypoints();

	void DragonProcessStart();
};
