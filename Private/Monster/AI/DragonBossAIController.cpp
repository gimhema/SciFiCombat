// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AI/DragonBossAIController.h"
#include "Monster/DragonBossBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

ADragonBossAIController::ADragonBossAIController()
{

}

void ADragonBossAIController::BeginPlay()
{
	Super::BeginPlay();

	controlled_dragon = Cast<ADragonBossBase>(GetPawn());
	DragonProcessStart();
}

void ADragonBossAIController::DragonProcessStart()
{
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			// Write Game Logic . . . .
			// 여기서 거리를 측정해서 상태를 결정하고
			//CheckDistance();
			// 결정된 상태에 따라 행동한다.
			DragonStateProcess();
		}), status_delay, true);
}

void ADragonBossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TravelWaypoints();
}

void ADragonBossAIController::DragonStateProcess()
{
	// 타겟 플레이어와의 거리에 따라 행동이 바뀐다.
	if (controlled_dragon->is_wakeup && controlled_dragon->is_death == false && controlled_dragon->target_player)
	{
		if (controlled_dragon->can_do_action)
		{
			SetFocus(controlled_dragon->target_player);
			controlled_dragon->can_do_action = false;

			switch (dragoun_status)
			{
			case EDragonStatus::EDS_MeleePhase:
				// 이동
				MoveToActor(controlled_dragon->target_player, follow_distance);
				// 카운트에 따라서
				if (bite_count >= bite_count_max)
				{
					// 물기 이후 페이즈 변경
					controlled_dragon->BiteAttack();
					bite_count = 0;
					SetDrgonStatus(EDragonStatus::EDS_RangePhase);
				}
				else if (bite_count == 1)
				{
					controlled_dragon->SmashAttack();
					bite_count++;
				}
				else
				{
					controlled_dragon->NormalAttack();
					bite_count++;
				}
				//controlled_dragon->can_do_action = true;
				break;
			case EDragonStatus::EDS_RangePhase:
				// 브레스 발사
				// 카운트에 따라서
				// 업졸브
				// 업졸브 이후 페이즈 변경
				if (spit_fire_count >= spit_fire_count_max)
				{
					controlled_dragon->Absorb();
					spit_fire_count = 0;
					SetDrgonStatus(EDragonStatus::EDS_MeleePhase);
				}
				else
				{
					controlled_dragon->SpitFireBall();
					spit_fire_count++;
				}
				//controlled_dragon->can_do_action = true;
				break;
			}
			//float _dist = FVector::Distance(controlled_dragon->target_player->GetActorLocation(),
			//	controlled_dragon->GetActorLocation());
			//if (_dist >= fire_distance)
			//{
			//	// 조건에 따라
			//	if (spit_fire_count >= spit_fire_count_max)
			//	{
			//		controlled_dragon->Absorb();
			//		spit_fire_count = 0;
			//	}
			//	else
			//	{
			//		controlled_dragon->SpitFireBall();
			//		spit_fire_count++;
			//	}
			//}
			//else if (_dist < fire_distance && _dist >= rush_distance)
			//{
			//	controlled_dragon->GetCharacterMovement()->MaxWalkSpeed = rush_speed;
			//	MoveToActor(controlled_dragon->target_player);
			//	controlled_dragon->can_do_action = true;
			//}
			//else if (_dist < rush_distance && _dist >= trace_distance)
			//{
			//	controlled_dragon->GetCharacterMovement()->MaxWalkSpeed = trace_speed;
			//	MoveToActor(controlled_dragon->target_player);
			//	controlled_dragon->can_do_action = true;
			//}
			//else if (_dist < attack_distance)
			//{
			//	// 조건에 따라
			//	if (bite_count >= bite_count_max)
			//	{
			//		controlled_dragon->BiteAttack();
			//		bite_count = 0;
			//	}
			//	else if(bite_count == 1)
			//	{
			//		controlled_dragon->SmashAttack();
			//		bite_count++;
			//	}
			//	else
			//	{
			//		controlled_dragon->NormalAttack();
			//		bite_count++;
			//	}
			//}
		}
	}
	
}

void ADragonBossAIController::SetDrgonStatus(EDragonStatus _status)
{
	dragoun_status = _status;
}

void ADragonBossAIController::MoveToWaypoint(FVector _Waypoint)
{
	MoveToLocation(_Waypoint);
}

void ADragonBossAIController::CheckDistance()
{
	

}


// Depercated
void ADragonBossAIController::TravelWaypoints()
{
	
}

bool ADragonBossAIController::CheckWaypoints()
{
	
	return false;
}


