// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SlapKnight_Legacy/Interfaces/I_Unit.h"

#include "BaseUnit.generated.h"

UCLASS()
class SLAPKNIGHT_LEGACY_API ABaseUnit : public APawn, public II_Unit
{
	GENERATED_BODY()

public:
	ABaseUnit();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void MoveForward() override;
	
protected:
	virtual void BeginPlay() override;

};
