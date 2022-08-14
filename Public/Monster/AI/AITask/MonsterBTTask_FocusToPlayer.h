// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MonsterBTTask_FocusToPlayer.generated.h"

/**
 * 
 * 
<������>

���¸� ������ ��Ÿ����.
�� ���� ���� �̸��� Monster State�������.

- Rush Base
  <��������> ���� �÷��̾ �߰��ߴ���? (Monster State = 0) -> �÷��̾� ���� ������ �Ǻ�
  <�½�ũ>
  - Find Base Location(����, Random Location ����)
  - Wait
  - Move To

- Chase Player
  <��������> ���� �÷��̾ �߰��ߴ���? (Monster State = 1) -> �÷��̾� ���� ������ �Ǻ�
  <�½�ũ>
  - Focus To Player (����)
  - Wait
  - Move To

- Attack Player ���� ���� �����ȿ� ���� ��������? (Monster State = 2)
  <��������>
  <�½�ũ>
  - Attack Player (����) -> AI Controller���� Attack �̺�Ʈ ȣ����
  - Check Attack State (����)

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
