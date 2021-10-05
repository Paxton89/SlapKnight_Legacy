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
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = Mesh;
	//Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	//Box->SetupAttachment(RootComponent);
	//Box->SetCollisionProfileName("BlockAllDynamic");
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

void ABaseTile::EstablishTileLegality() // Checks if this tile is empty of units and if the current unit can afford to move here, is so, its a legal tile to move to.
{
	bool hasEnoughStamina = !CurrentUnit && Tile(gameMode->currentTile)->CurrentUnit->CurrentStamina >= costToMove;
	IsLegalTile(hasEnoughStamina);
}

void ABaseTile::IsLegalTile(bool legal) // Sets this tiles variables as legal or not legal depending on the boolean.
{
	affordable = legal;
	legalTile = legal;
	HighlightedPlaneGreen->SetVisibility(legal);
}

bool ABaseTile::GetUnitTeam() // Returns the team of the unit standing on this tile.
{
	return CurrentUnit->teamBlue;
}

int ABaseTile::GetUnitStamina() // Returns the stamina of the unit standing on this tile
{
	return CurrentUnit->CurrentStamina;
}

void ABaseTile::RiseTile(int offset)
{
	FVector risenPos = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + offset);
	SetActorLocation(risenPos);
	if (CurrentUnit)
	{
		FVector risenPos2 = FVector(CurrentUnit->GetActorLocation().X, CurrentUnit->GetActorLocation().Y, CurrentUnit->GetActorLocation().Z + offset);
		CurrentUnit->SetActorLocation(risenPos2);
	}

}

void ABaseTile::SelectTile() // If this tile has a unit on top then it becomes selected, highlighted and becomes the currentTile of the gameMode.
{
	if (CurrentUnit && Cast<ABaseUnit>(CurrentUnit)->teamBlue == gameMode->teamBlue)
	{
		HighlightedPlaneYellow->SetVisibility(true);
		gameMode->currentTile = TileId; 
		selected = true;
		ActivateNeighbours(true);
	}
}

void ABaseTile::DeSelectTile() // Sets this tiles variables as deselected.
{
	HighlightedPlaneYellow->SetVisibility(false);
	selected = false;
	ActivateNeighbours(false);
}

void ABaseTile::UpdateNeighbours() // Makes an array of the 4 neighbouring tiles
{
	neighbours.Empty();
	for (ABaseTile* tile : gameMode->allTiles)
	{
		bool leftTile = tile->pos.X == pos.X - 1 && tile->pos.Y == pos.Y;
		bool rightTile = tile->pos.X == pos.X + 1 && tile->pos.Y == pos.Y;
		bool upTile = tile->pos.X == pos.X && tile->pos.Y == pos.Y + 1;
		bool downTile = tile->pos.X == pos.X && tile->pos.Y == pos.Y - 1;
		if ( upTile || downTile || rightTile || leftTile)
			neighbours.Add(tile->TileId);
	}
}

void ABaseTile::ActivateNeighbours(bool activate)// This activates or deactivates neighbouring tiles, also deals with their highlight.
{
	for (int tile : neighbours)
	{
		
		Tile(tile)->activated = activate;
		Tile(tile)->HighlightedPlaneGreen->SetVisibility(activate);
		if (activate)
			Tile(tile)->EstablishTileLegality();
		else
			Tile(tile)->IsLegalTile(false);
	}
}


ABaseTile* ABaseTile::Tile(int Tile)
{
	return gameMode->allTiles[Tile];
}

