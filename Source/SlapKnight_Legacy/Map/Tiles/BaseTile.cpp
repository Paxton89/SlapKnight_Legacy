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
	HighlightPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("highlight"));
	HighlightPlane->SetupAttachment(RootComponent);
	TargetToMove = CreateDefaultSubobject<USceneComponent>(TEXT("TargetToMove"));
	TargetToMove->SetupAttachment(RootComponent);
}

void ABaseTile::BeginPlay() // Finds neighbours and game mode for future reference.
{
	Super::BeginPlay();
	GameMode = Cast<ASlapKnight_LegacyGameModeBase>(GetWorld()->GetAuthGameMode());
	UpdateNeighbours();
	ChangeHighlightToGreen();
}

void ABaseTile::Tick(float DeltaTime) // Currently empty, good :D.
{
	Super::Tick(DeltaTime);
}

void ABaseTile::EstablishTileLegality() // Checks if this tile is empty of units and if the current unit can afford to move here, is so, its a legal tile to move to.
{
	bool hasEnoughStamina = !CurrentUnit && Tile(GameMode->currentTile)->CurrentUnit->CurrentStamina >= CostToMove;
	IsLegalTile(hasEnoughStamina);
}

void ABaseTile::IsLegalTile(bool legal) // Sets this tiles variables as legal or not legal depending on the boolean.
{
	Affordable = legal;
	LegalTile = legal;
	if(HighlightPlane != nullptr)
		HighlightPlane->SetVisibility(legal);
}

bool ABaseTile::GetUnitTeam() // Returns the team of the unit standing on this tile.
{
	return CurrentUnit->TeamBlue;
}

int ABaseTile::GetUnitStamina() // Returns the stamina of the unit standing on this tile
{
	return CurrentUnit->CurrentStamina;
}

void ABaseTile::RiseTile(int offset)
{
	FVector risenPos = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + offset);
	SetActorLocation(risenPos);
}

void ABaseTile::SelectTile() // If this tile has a unit on top then it becomes selected, highlighted and becomes the currentTile of the gameMode.
{
	//if (CurrentUnit && Cast<ABaseUnit>(CurrentUnit)->teamBlue == gameMode->teamBlue)
	if (CurrentUnit)
	{
		if (Cast<ABaseUnit>(CurrentUnit)->teamBlue == GameMode->TeamBlue)
		{
			if (HighlightPlane != nullptr)
				HighlightPlane->SetVisibility(true);
			GameMode->currentTile = TileId;
			Selected = true;
			ActivateNeighbours(true);
		}
		else
		{
			if (HighlightPlane != nullptr)
				HighlightPlane->SetVisibility(true);
			
			GameMode->currentTile = TileId;
			Selected = true;
		}

		ChangeHighlightToYellow();
	}
}

void ABaseTile::DeSelectTile() // Sets this tiles variables as deselected.
{
	if (HighlightPlane != nullptr)
		HighlightPlane->SetVisibility(false);
	Selected = false;
	ChangeHighlightToGreen();
	ActivateNeighbours(false);
}

void ABaseTile::UpdateNeighbours() // Makes an array of the 4 neighbouring tiles
{
	Neighbours.Empty();
	for (ABaseTile* tile : GameMode->allTiles)
	{
		bool leftTile = tile->Pos.X == Pos.X - 1 && tile->Pos.Y == Pos.Y;
		bool rightTile = tile->Pos.X == Pos.X + 1 && tile->Pos.Y == Pos.Y;
		bool upTile = tile->Pos.X == Pos.X && tile->Pos.Y == Pos.Y + 1;
		bool downTile = tile->Pos.X == Pos.X && tile->Pos.Y == Pos.Y - 1;
		if ( upTile || downTile || rightTile || leftTile)
			Neighbours.Add(tile->TileId);
	}
}

void ABaseTile::ActivateNeighbours(bool activate)// This activates or deactivates neighbouring tiles, also deals with their highlight.
{
	for (int tile : Neighbours)
	{
		
		Tile(tile)->Activated = activate;

		if (Tile(tile)->HighlightPlane != nullptr)
			Tile(tile)->HighlightPlane->SetVisibility(activate);
		if (activate)
		{
			ChangeHighlightToGreen();
			Tile(tile)->EstablishTileLegality();
		}
		else
		{
			Tile(tile)->IsLegalTile(false);
		}

	}
}


ABaseTile* ABaseTile::Tile(int Tile)
{
	return GameMode->allTiles[Tile];
}

