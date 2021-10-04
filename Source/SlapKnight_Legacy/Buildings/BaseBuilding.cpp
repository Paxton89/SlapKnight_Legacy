// J


#include "SlapKnight_Legacy//Buildings/BaseBuilding.h"
#include "SlapKnight_Legacy/Units/BaseUnit.h"

ABaseBuilding::ABaseBuilding()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseBuilding::SpawnUnit()
{
	//Spawn Unit
}

