// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem/AmmoPickupItem.h"
#include "SciFiCombat/Public/Character/CombatCharacter.h"
#include "SciFiCombat/Public/CombatComponent/CombatComponent.h"

void AAmmoPickupItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool isFromSweep, const FHitResult& SweepResult
)

{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, isFromSweep, SweepResult);

	ACombatCharacter* combat_character = Cast<ACombatCharacter>(OtherActor);
	if (combat_character)
	{
		UCombatComponent* combat_component = combat_character->GetCombatComponent();
		if (combat_component)
		{
			combat_component->PickupAmmoItem(weapon_type, num_ammo);
		}
	}
	Destroy();
}
