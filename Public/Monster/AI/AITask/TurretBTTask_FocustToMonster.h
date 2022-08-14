// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "TurretBTTask_FocustToMonster.generated.h"

/**
 * 
 */
UCLASS()
class SCIFICOMBAT_API UTurretBTTask_FocustToMonster : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UTurretBTTask_FocustToMonster();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	float serachradius{ 500.f };
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
