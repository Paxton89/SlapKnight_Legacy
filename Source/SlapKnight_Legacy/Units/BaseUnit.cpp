
#include "SlapKnight_Legacy/Units/BaseUnit.h"


ABaseUnit::ABaseUnit()
{

	PrimaryActorTick.bCanEverTick = true;

}


void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseUnit::MoveForward()
{
}

