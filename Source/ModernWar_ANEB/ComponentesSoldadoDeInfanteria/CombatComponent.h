// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MODERNWAR_ANEB_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	friend class ASoldadoDeInfanteria;

	void EquipWeapon(class AWeapon* WeaponToEquip);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetAiming(bool IsItAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool IsItAiming);

private:
	class ASoldadoDeInfanteria* SoldadoDeInfanteria;

	UPROPERTY(Replicated)
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool IsAiming;
		
};
