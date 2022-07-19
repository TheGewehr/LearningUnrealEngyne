// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoldadoDeInfanteria.generated.h"

UCLASS()
class MODERNWAR_ANEB_API ASoldadoDeInfanteria : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoldadoDeInfanteria();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void MooveForward(float value);
	void MooveRight(float value);
	void Turn(float value);
	void LookUp(float value);

private:	
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;
public:
};
