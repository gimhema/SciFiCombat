// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CombatAnimInstance.h"
#include "Character/CombatCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "SciFiCombat/SciFiCombatType/CombatStatge.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCombatAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
}

void UCombatAnimInstance::NativeUpdateAnimation(float delta)
{
	Super::NativeUpdateAnimation(delta);

	if (CombatCharacter == nullptr)
	{
		CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
	}
	if (CombatCharacter == nullptr) return;

	FVector velocity = CombatCharacter->GetVelocity();
	velocity.Z = 0.f;
	speed = velocity.Size();

	isJumping = CombatCharacter->is_jumping;
	isAir = CombatCharacter->GetCharacterMovement()->IsFalling();
	isAccel = CombatCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	isWeaponEquiped = CombatCharacter->IsWeaponEquiped();
	isCrouched = CombatCharacter->bIsCrouched;
	isAim = CombatCharacter->IsAim();
	turing_state = CombatCharacter->GetTurningState();
	is_death = CombatCharacter->IsDeath();

	FRotator aim_rotation = CombatCharacter->GetBaseAimRotation();
	FRotator movement_rotation = UKismetMathLibrary::MakeRotFromX(CombatCharacter->GetVelocity());
	FRotator delta_rot = UKismetMathLibrary::NormalizedDeltaRotator(movement_rotation, aim_rotation);
	delta_rotation = FMath::RInterpTo(delta_rotation, delta_rot, delta, 6.f);
	yaw_offset = delta_rotation.Yaw;
	
	character_rotation_last_frame = character_rotation;
	character_rotation = CombatCharacter->GetActorRotation();
	const FRotator _delta = UKismetMathLibrary::NormalizedDeltaRotator(character_rotation, character_rotation_last_frame);
	const float target = _delta.Yaw / delta;
	const float interp = FMath::FInterpTo(lean, target, delta, 6.f);
	lean = FMath::Clamp(interp, -90.f, 90.f);

	aim_offset_yaw = CombatCharacter->GetAimOffsetYaw();
	aim_offset_pitch = CombatCharacter->GetAimOffsetPitch();
}

// UE_LOG(LogTemp, Warning, TEXT("Content . . ."), AimRotation.Yaw);