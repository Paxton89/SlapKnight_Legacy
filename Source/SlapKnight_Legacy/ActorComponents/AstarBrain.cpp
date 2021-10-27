#include "AstarBrain.h"
#include "../SlapKnight_LegacyGameModeBase.h"
#include "../Units/BaseUnit.h"
#include "../Map/Tiles/BaseTile.h"

UAstarBrain::UAstarBrain()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAstarBrain::SetStartTile(int startTile) // Resets everything, and establishes this tile as the starting tile.
{
	ResetEndStartTile();
	CurrentStart = startTile;
}

void UAstarBrain::ResetEndStartTile() // Resets all lists, start and end tiles.
{
	CurrentEnd = -1;
	Path.Empty();
	CorrectPath.Empty();
	ClosedList.Empty();
	OpenList.Empty();
	Path.Empty();
}

void UAstarBrain::SetEndTile(int endTile) // This sets the end tile and gets a path there from start tile.
{
	ResetEndStartTile();
	if (Tile(endTile)->CurrentUnit == nullptr && CurrentStart != -1)
	{
		CurrentEnd = endTile;
		ClosedList.Empty();
		ClosedList.Add(CurrentTile);
		Path = Calculatepath(CurrentStart, CurrentEnd);
		UpdateHighlights(Path);
	}
}

TArray<int> UAstarBrain::Calculatepath(int startTile, int endTile)
{
	//---------------------------------------SetUp---------------------------------------
	TArray<int> currentCorrectPath;
	OpenList.AddUnique(startTile);
	CurrentTile = startTile;
	currentCorrectPath.Add(CurrentTile);
	UpdateATilesStats(CurrentTile, startTile, endTile);
	//-----------------------------------Calculate Path----------------------------------
	while (OpenList.Num() > 0)
	{
		//-------------------------Add Neighbors to Open list----------------------------
		TArray<int> neighbors = Tile(startTile)->Neighbours;;
		for (auto tile : neighbors)
		{
			if (OpenList.Contains(tile))
				continue;
			OpenList.AddUnique(tile);
			UpdateATilesStats(tile, startTile, endTile);
		}
		//-----------------------Move current to correct list----------------------------
		OpenList.Remove(CurrentTile);
		ClosedList.AddUnique(CurrentTile);
		//--------------------------Chose next tile to visit-----------------------------
		int myF = INT_MAX;
		for (int i = 0; i < OpenList.Num(); i++)
		{
			int newF = Tile(OpenList[i])->F;
			if (newF <= myF)
			{
				myF = newF;
				CurrentTile = OpenList[i];
			}
		}
		currentCorrectPath.Add(CurrentTile);
		//-----------------------------End of path found---------------------------------
		if (CurrentTile == endTile)
			break;
	}
	//---------------------------------Finishing touches---------------------------------
	currentCorrectPath = CalculateCorrectPath(startTile, endTile);
	Algo::Reverse(currentCorrectPath);
	return currentCorrectPath;
}

void UAstarBrain::UpdateATilesStats(int updateTile, int startTile, int endTile) // Updates 1 tile to a new H, G and F value.
{
	if (endTile != -1)
	{
		if (Tile(updateTile)->CurrentUnit == nullptr)
		{
			Tile(updateTile)->H = CalculateH(Tile(updateTile)->TileId, endTile);
			CalculateG(startTile, Tile(updateTile)->TileId);
			Tile(updateTile)->F = Tile(updateTile)->H + Tile(updateTile)->G;
		}
	}
}

int UAstarBrain::CalculateH(int startTile, int endTile) //Calculates distance to end.
{
	FVector startPos = Tile(startTile)->Pos;
	FVector EndPos =  Tile(endTile)->Pos;
	int hCost = 0;
	int hCostX = abs(startPos.X - EndPos.X);
	int hCostY = abs(startPos.Y - EndPos.Y);

	hCost = (hCostX + hCostY) * 10; // only for no diagonal movement
					
	/* this is for diagonal movement
	hCost += abs(hCostX - hCostY) * 10;
	if (hCostX < hCostY) { hCost += hCostX * 15; }
	else { hCost += hCostY * 15; }
	*/
	return hCost;
}

void UAstarBrain::CalculateG(int startTile, int currentTile) // Calculates cost to start.
{
	int g = 0;
	g += Tile(startTile)->CostToMove;
	g += Tile(startTile)->CombinedCostToMove;
	if (currentTile == startTile)
	{
		Tile(currentTile)->Parent = currentTile;
		Tile(currentTile)->CombinedCostToMove = g;
		Tile(currentTile)->G = g;
	}
	if (Tile(currentTile)->Parent == -1)
	{
		Tile(currentTile)->Parent = currentTile;
		Tile(currentTile)->CombinedCostToMove = g;
		Tile(currentTile)->G = g;
	}
}

TArray<int> UAstarBrain::CalculateCorrectPath(int startTile, int endTile) // We make a path from endTile to startTile by going through the parents on each tile. this reversed is the correct path.
{
	TArray<int> theCorrectPath;
	int currentNode = endTile;
	theCorrectPath.Add(currentNode);
	while (!theCorrectPath.Contains(startTile))
	{
		if (Tile(currentNode)->Parent == -1) { break; }
		currentNode = Tile(currentNode)->Parent;
		theCorrectPath.Add(currentNode);
	}
	UE_LOG(LogTemp, Warning, TEXT("path found! "));
	return theCorrectPath;
}

void UAstarBrain::UpdateHighlights(TArray<int> finalPath) //makes sure the appropriate visual highlight are shown for each tile.
{
	for (auto tile : finalPath)
	{
		Tile(tile)->ChangeHighlightToRed();
	}
}

int UAstarBrain::CalculateFinalCost(TArray<int> finalPath) // This returns the final cost of movement of this path, useful to compare paths, currently not used.
{
	int finalCost = 0;
	finalCost = Tile(finalPath[0])->CombinedCostToMove;
	UE_LOG(LogTemp, Warning, TEXT("Path:  %d final cost: %d   "), finalPath[0], finalCost);
	return finalCost;
}

ABaseTile* UAstarBrain::Tile(int TileId) // This is here to shorten GameMode->allTiles[TileId] into Tile(TileId) since its used a lot.
{
	return GameMode->allTiles[TileId];
}