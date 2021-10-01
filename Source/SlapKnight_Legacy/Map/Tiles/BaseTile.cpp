
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "Components/SceneComponent.h"
#include "../../Units/BaseUnit.h"
#include "../../SlapKnight_LegacyGameModeBase.h"
#include "Components/StaticMeshComponent.h"

ABaseTile::ABaseTile()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	HighlightedPlaneGreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("highlightGreen"));
	HighlightedPlaneYellow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("highlightYellow"));
	HighlightedPlaneGreen->SetupAttachment(RootComponent);
	HighlightedPlaneYellow->SetupAttachment(RootComponent);
	TargetToMove = CreateDefaultSubobject<USceneComponent>(TEXT("TargetToMove"));
	TargetToMove->SetupAttachment(RootComponent);
}

void ABaseTile::BeginPlay()
{
	Super::BeginPlay();
	gameMode = Cast<ASlapKnight_LegacyGameModeBase>(GetWorld()->GetAuthGameMode());
}

void ABaseTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (legalTile)
	{
		HighlightedPlaneGreen->SetVisibility(true);
	}
	else
	{
		HighlightedPlaneGreen->SetVisibility(false);
	}	
	if (selected)
	{
		HighlightedPlaneYellow->SetVisibility(true);
	}
	else
	{
		HighlightedPlaneYellow->SetVisibility(false);
	}
}

void ABaseTile::EstablishTileLegality()
{
	if (activated && affordable && CurrentUnit == nullptr) 
	{ 
		legalTile = true; 
		HighlightedPlaneGreen->SetVisibility(true);
	}
	else 
	{ 
		legalTile = false; 
		HighlightedPlaneGreen->SetVisibility(false);
	}
}

void ABaseTile::GetUnitTeam()
{
}

void ABaseTile::GetUnitStamina()
{
}

void ABaseTile::SelectTile()
{
	
	if (CurrentUnit)
	{
		if (Cast<ABaseUnit>(CurrentUnit)->teamBlue == gameMode->teamBlue)
		{
			selected = true;
			gameMode->currentTile = this; //This is now our CurrentTile
		}
	}
}

void ABaseTile::DeSelectTile()
{
	if (CurrentUnit != nullptr && Cast<ABaseUnit>(CurrentUnit)->teamBlue == gameMode->teamBlue)
	{
		selected = false;
	}
}

