// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldadoDeInfanteriaAnimInstance.h"
#include "SoldadoDeInfanteria.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}
