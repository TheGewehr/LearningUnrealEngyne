// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "ModernWar_ANEB/Weapons/Weapon.h"
#include "ModernWar_ANEB/SoldadoDeInfanteria.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (SoldadoDeInfanteria)
	{
		SoldadoDeInfanteria->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
	
}

void UCombatComponent::SetAiming(bool IsItAiming)
{
	IsAiming = IsItAiming; // Set locally

	// What is commented is not necessary
	//if (!SoldadoDeInfanteria->HasAuthority()) // I it is not the server
	//{
	ServerSetAiming(IsItAiming); // Set to the authority (Server)
	//}


	if (SoldadoDeInfanteria)
	{
		SoldadoDeInfanteria->GetCharacterMovement()->MaxWalkSpeed = IsItAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool IsItAiming)
{
	IsAiming = IsItAiming;

	if (SoldadoDeInfanteria)
	{
		SoldadoDeInfanteria->GetCharacterMovement()->MaxWalkSpeed = IsItAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && SoldadoDeInfanteria)
	{
		SoldadoDeInfanteria->GetCharacterMovement()->bOrientRotationToMovement = false;
		SoldadoDeInfanteria->bUseControllerRotationYaw = true;

	}
}




// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (SoldadoDeInfanteria == nullptr || WeaponToEquip == nullptr)
	{
		return;
	}

	EquippedWeapon = WeaponToEquip;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	const USkeletalMeshSocket* RightHandSocket = SoldadoDeInfanteria->GetMesh()->GetSocketByName(FName("RightHandSocket"));

	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(EquippedWeapon, SoldadoDeInfanteria->GetMesh());
	}

	EquippedWeapon->SetOwner(SoldadoDeInfanteria);

	
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, IsAiming);
}

