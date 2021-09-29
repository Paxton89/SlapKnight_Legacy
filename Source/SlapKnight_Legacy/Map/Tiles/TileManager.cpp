#include "TileManager.h"
#include "Kismet/GameplayStatics.h"

UTileManager::UTileManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTileManager::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseTile::StaticClass(), allActorTiles);
	for (auto tile : allActorTiles)
	{
		int x = tile->GetActorLocation().X / 100;
		int y = tile->GetActorLocation().Y / 100;
		Cast<ABaseTile>(tile)->pos.X = -x;
		Cast<ABaseTile>(tile)->pos.Y = -y;
		allTiles.Add(Cast<ABaseTile>(tile));
		Cast<ABaseTile>(tile)->tileId = allTiles.Num()-1;
	}
	for (ABaseTile* tile : allTiles)
	{
		int x = tile->pos.X;
		int y = tile->pos.Y;
		UE_LOG(LogTemp, Warning, TEXT("Tile Id: %d Tile Pos: (%d,%d)"), tile->tileId, x, y);
	}
}

void UTileManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

