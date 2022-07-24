// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldadoDeInfanteriaAnimInstance.h"
#include "SoldadoDeInfanteria.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void USoldadoDeInfanteriaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SoldadoDeInfanteria = Cast<ASoldadoDeInfanteria>(TryGetPawnOwner());
}

void USoldadoDeInfanteriaAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SoldadoDeInfanteria == nullptr)
	{
		SoldadoDeInfanteria = Cast<ASoldadoDeInfanteria>(TryGetPawnOwner());
	}

	if (SoldadoDeInfanteria == nullptr)
	{
		return;
	}

	FVector Velocity = SoldadoDeInfanteria->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	IsInAir = SoldadoDeInfanteria->GetCharacterMovement()->IsFalling();

	IsAccelerating = SoldadoDeInfanteria->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	WeaponEquipped = SoldadoDeInfanteria->IsWeaponEquipped();

	IsCrouching = SoldadoDeInfanteria->bIsCrouched;

	IsAiming = SoldadoDeInfanteria->IsSoldierAiming();

	// Offset Yaw for Strafing
	FRotator AimRotation = SoldadoDeInfanteria->GetBaseAimRotation(); //Global Rotation
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(SoldadoDeInfanteria->GetVelocity()); // Local Rotation
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 15.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = SoldadoDeInfanteria->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
}
