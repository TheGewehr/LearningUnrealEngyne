// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldadoDeInfanteria.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASoldadoDeInfanteria::ASoldadoDeInfanteria()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bool UseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ASoldadoDeInfanteria::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ASoldadoDeInfanteria::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("Move Forward", this, &ASoldadoDeInfanteria::MooveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &ASoldadoDeInfanteria::MooveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ASoldadoDeInfanteria::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASoldadoDeInfanteria::LookUp);
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

// Called every frame
void ASoldadoDeInfanteria::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



