
#include "SlapKnight_Legacy/Map/BaseTile.h"
#include "Components/StaticMeshComponent.h"

ABaseTile::ABaseTile()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ABaseTile::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

