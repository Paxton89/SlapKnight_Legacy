
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "Components/SceneComponent.h"
#include "../../Units/BaseUnit.h"
#include "../../SlapKnight_LegacyGameModeBase.h"
#include "Components/BoxComponent.h"
#include "SlapKnight_Legacy/Map/Tiles/TileManager.h"
#include "Components/StaticMeshComponent.h"

ABaseTile::ABaseTile() // Creates the root, the targetToMove that is the position where units will move to and it creates 2 planes used for highlighting.
{
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = Box;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("NoCollision");
	HighlightedPlaneGreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("highlightGreen"));
	HighlightedPlaneYellow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("highlightYellow"));
	HighlightedPlaneGreen->SetupAttachment(RootComponent);
	HighlightedPlaneYellow->SetupAttachment(RootComponent);
	TargetToMove = CreateDefaultSubobject<USceneComponent>(TEXT("TargetToMove"));
	TargetToMove->SetupAttachment(RootComponent);
}

void ABaseTile::BeginPlay() // Finds neighbours and game mode for future reference.
{
	Super::BeginPlay();
	gameMode = Cast<ASlapKnight_LegacyGameModeBase>(GetWorld()->GetAuthGameMode());
	UpdateNeighbours();
}

void ABaseTile::Tick(float DeltaTime) // Currently empty, good :D.
{
	Super::Tick(DeltaTime);
}

void ABaseTile::EstablishTileLegality() // Checks if this tile is activated, if is empty of units and if the current unit can afford to move here, is so, its a legal tile to move to.
{
	affordable = gameMode->currentTile->CurrentUnit->CurrentStamina >= costToMove; 
	legalTile = activated && affordable && CurrentUnit == nullptr;
	HighlightedPlaneGreen->SetVisibility(activated && affordable && CurrentUnit == nullptr);
}

bool ABaseTile::GetUnitTeam() // Returns the team of the unit standing on this tile.
{
	return CurrentUnit->teamBlue;
}

int ABaseTile::GetUnitStamina() // Returns the stamina of the unit standing on this tile
{
	return CurrentUnit->CurrentStamina;
}

void ABaseTile::SelectTile() // If this tile has a unit on top then it becomes selected, highlighted and becomes the currentTile of the gameMode.
{
	if (CurrentUnit && Cast<ABaseUnit>(CurrentUnit)->teamBlue == gameMode->teamBlue)
	{
		selected = true;
		HighlightedPlaneYellow->SetVisibility(true);
		gameMode->currentTile = this; // This is now our CurrentTile
		ActivateNeighbours(true);
	}
}

void ABaseTile::DeSelectTile() // If this tile has a unit on top then it becomes selected, highlighted and becomes the currentTile of the gameMode.
{
	if (CurrentUnit && Cast<ABaseUnit>(CurrentUnit)->teamBlue == gameMode->teamBlue)
	{
		selected = false;
		HighlightedPlaneYellow->SetVisibility(false);
		ActivateNeighbours(false);
	}
}

void ABaseTile::UpdateNeighbours() // Makes an array of the 4 neighbouring tiles
{
	neighbours.Empty();
	for (ABaseTile* tile : gameMode->allTiles)
	{
		if (tile->pos.X == pos.X && tile->pos.Y == pos.Y + 1 || tile->pos.X == pos.X && tile->pos.Y == pos.Y - 1 || tile->pos.X == pos.X + 1 && tile->pos.Y == pos.Y || tile->pos.X == pos.X - 1 && tile->pos.Y == pos.Y)
		{
			neighbours.Add(tile);
		}
	}
}

void ABaseTile::ActivateNeighbours(bool activate)// This activates or deactivates neighbouring tiles, also dealing with their highlight.
{
	for (ABaseTile* tile : neighbours)
	{
		tile->activated = activate;
		tile->HighlightedPlaneGreen->SetVisibility(activate);
		tile->EstablishTileLegality();
	}
}


