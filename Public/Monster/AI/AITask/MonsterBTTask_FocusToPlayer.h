// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MonsterBTTask_FocusToPlayer.generated.h"

/**
 * 
 * 
<시퀀스>

상태를 정수로 나타낸다.
이 상태 정수 이름을 Monster State라고하자.

- Rush Base
  <진입조건> 현재 플레이어를 발견했는지? (Monster State = 0) -> 플레이어 액터 감지로 판별
  <태스크>
  - Find Base Location(구현, Random Location 변형)
  - Wait
  - Move To

- Chase Player
  <진입조건> 현재 플레이어를 발견했는지? (Monster State = 1) -> 플레이어 액터 감지로 판별
  <태스크>
  - Focus To Player (구현)
  - Wait
  - Move To

- Attack Player 현재 공격 범위안에 들어온 상태인지? (Monster State = 2)
  <진입조건>
  <태스크>
  - Attack Player (구현) -> AI Controller에서 Attack 이벤트 호출함
  - Check Attack State (구현)

- Wait
 * 
 */
UCLASS()
class SCIFICOMBAT_API UMonsterBTTask_FocusToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UMonsterBTTask_FocusToPlayer();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
		float serachradius{ 500.f };
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
