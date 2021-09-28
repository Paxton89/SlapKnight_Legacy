
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
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

