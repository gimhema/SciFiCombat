// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BossBTTask_SelectTarget.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UBossBTTask_SelectTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBossBTTask_SelectTarget();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
		float serachradius{ 500.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
		float phase_switch{ 15.f };
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};

