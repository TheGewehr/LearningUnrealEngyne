// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldadoDeInfanteria.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/Weapon.h"
#include "ComponentesSoldadoDeInfanteria/CombatComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASoldadoDeInfanteria::ASoldadoDeInfanteria()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600;
	CameraBoom->bUsePawnControlRotation = true;
	//bUseControllerRotationYaw = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bool UseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	Combat->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	
}

void ASoldadoDeInfanteria::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASoldadoDeInfanteria, OverlappingWeapon, COND_OwnerOnly);
}



// Called when the game starts or when spawned
void ASoldadoDeInfanteria::BeginPlay()
{
	Super::BeginPlay();
	bUseControllerRotationYaw = true;
	
}

// Called every frame
void ASoldadoDeInfanteria::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ASoldadoDeInfanteria::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ThisClass::EquipButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ThisClass::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ThisClass::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ThisClass::AimButtonReleased);

	PlayerInputComponent->BindAxis("Move Forward", this, &ASoldadoDeInfanteria::MooveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &ASoldadoDeInfanteria::MooveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ASoldadoDeInfanteria::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASoldadoDeInfanteria::LookUp);
}

void ASoldadoDeInfanteria::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Combat)
	{
		Combat->SoldadoDeInfanteria = this;
	}
}


void ASoldadoDeInfanteria::MooveForward(float value)
{
	if (Controller != nullptr && value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));

		AddMovementInput(Direction, value);
	}

	
}

void ASoldadoDeInfanteria::MooveRight(float value)
{
	if (Controller != nullptr && value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

		AddMovementInput(Direction, value);
	}
}

void ASoldadoDeInfanteria::Turn(float value)
{
	AddControllerYawInput(value);
}

void ASoldadoDeInfanteria::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ASoldadoDeInfanteria::EquipButtonPressed() // The server is the only one to has the Authority to call this function
{
	if (Combat)
	{
		if (HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
		
	}

	
}

void ASoldadoDeInfanteria::CrouchButtonPressed()
{
	if (bIsCrouched == false)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
	

}

void ASoldadoDeInfanteria::AimButtonPressed()
{
	if (Combat)
	{
		Combat->SetAiming(true);
	}
}

void ASoldadoDeInfanteria::AimButtonReleased()
{
	if (Combat)
	{
		Combat->SetAiming(false);
	}
}

void ASoldadoDeInfanteria::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ASoldadoDeInfanteria::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(false);
	}

	OverlappingWeapon = Weapon;

	if (IsLocallyControlled()) // Only the local must be able to see it
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickUpWidget(true);
		}
	}
}

bool ASoldadoDeInfanteria::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool ASoldadoDeInfanteria::IsSoldierAiming()
{
	return (Combat && Combat->IsAiming);
}

void ASoldadoDeInfanteria::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(true);
	}

	if (LastWeapon)
	{
		LastWeapon->ShowPickUpWidget(false);
	}
}


