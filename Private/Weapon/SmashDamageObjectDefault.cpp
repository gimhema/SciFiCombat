// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SmashDamageObjectDefault.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/SciFiCombat.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "SciFiCombat/Public/Monster/MonsterBase.h"

// Sets default values
ASmashDamageObjectDefault::ASmashDamageObjectDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	hit_area = CreateDefaultSubobject<UBoxComponent>(TEXT("HitArea"));
	SetRootComponent(hit_area);
	hit_area->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	hit_area->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	hit_area->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	//hit_area->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	//hit_area->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void ASmashDamageObjectDefault::BeginPlay()
{
	Super::BeginPlay();
	
	if (hit_sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			hit_sound,
			GetActorLocation()
		);
	}

	if (HasAuthority())
	{
		hit_area->OnComponentHit.AddDynamic(this, &ASmashDamageObjectDefault::OnHit);
	}
	

	FTimerHandle destroy_handle;
	GetWorld()->GetTimerManager().SetTimer(destroy_handle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), destroy_delay, false); //반복도 여기서 추가 변수를 선언해 설정가능
}

void ASmashDamageObjectDefault::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImulse, const FHitResult& Hit)
{
	// Other Actor를 몬스터 Class로 형변환하여 사용 
	
	bool monster_valid = IsValid(Cast<AMonsterBase>(OtherActor));

	ACharacter* owner_character = Cast<ACharacter>(GetOwner());
	if (owner_character && monster_valid)
	{
		AController* owner_controller = owner_character->Controller;
		if (owner_controller)
		{
			UGameplayStatics::ApplyDamage(OtherActor, obj_damage, owner_controller, this, UDamageType::StaticClass());
		}
	}
}

// Called every frame
void ASmashDamageObjectDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmashDamageObjectDefault::Destroyed()
{
	Super::Destroyed();

}
