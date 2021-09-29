
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "Components/SceneComponent.h"

#include "Components/StaticMeshComponent.h"

ABaseTile::ABaseTile()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	HighlightedPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("highlight"));
	TargetToMove = CreateDefaultSubobject<USceneComponent>(TEXT("TargetToMove"));
	TargetToMove->SetupAttachment(RootComponent);
	HighlightedPlane->SetupAttachment(RootComponent);
}

void ABaseTile::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (legalTile)
	{
		HighlightedPlane->SetVisibility(true);
	}
	else
	{
		HighlightedPlane->SetVisibility(false);
	}
}

void ABaseTile::EstablishTileLegality()
{
	if (activated && affordable && occupied == nullptr) 
	{ 
		legalTile = true; 
		HighlightedPlane->SetVisibility(true);
	}
	else 
	{ 
		legalTile = false; 
		HighlightedPlane->SetVisibility(false);
	}
}

