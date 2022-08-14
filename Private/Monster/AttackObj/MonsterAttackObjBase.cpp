// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AttackObj/MonsterAttackObjBase.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"

// Sets default values
AMonsterAttackObjBase::AMonsterAttackObjBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	melee_hit_area = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeHitBox"));
	melee_hit_area->SetupAttachment(RootComponent);
	melee_hit_area->OnComponentBeginOverlap.AddDynamic(this, &AMonsterAttackObjBase::OverlapMeleeHitArea);
}

// Called when the game starts or when spawned
void AMonsterAttackObjBase::BeginPlay()
{
	Super::BeginPlay();
	
	//if (HasAuthority()) // Same Meaning -> GetLocalRole() == ENetRole::ROLE_Authority
	//{
	//	melee_hit_area->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OverlapMeleeHitArea);
	//}

	//melee_hit_area->OnComponentBeginOverlap.AddDynamic(this, &AMonsterAttackObjBase::OverlapMeleeHitArea);
	FTimerHandle wait_handle;
	GetWorld()->GetTimerManager().SetTimer(wait_handle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), destroy_delay, false);
}

// Called every frame
void AMonsterAttackObjBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterAttackObjBase::OverlapMeleeHitArea(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool isFromSweep,
	const FHitResult& SweepResult)
{
	bool target_player_valid = IsValid(Cast<ACombatCharacter>(OtherActor));

	if (target_player_valid)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Player Valid"));
		UGameplayStatics::ApplyDamage(OtherActor, obj_dmg, nullptr, this, UDamageType::StaticClass());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Player Not Valid"));
		return;
	}
}