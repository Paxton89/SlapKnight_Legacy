// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlapKnight_LegacyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Map/Tiles/BaseTile.h"
#include "Map/Tiles/TileManager.h"

ASlapKnight_LegacyGameModeBase::ASlapKnight_LegacyGameModeBase()
{
}

void ASlapKnight_LegacyGameModeBase::BeginPlay() // This makes a list of all all already casted tiles and assignes them a position.
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
		Cast<ABaseTile>(tile)->TileId = allTiles.Num() - 1;
	}
	playerController = GetWorld()->GetFirstPlayerController();
	playerController->bShowMouseCursor = true;
}

APlayerController* ASlapKnight_LegacyGameModeBase::GetPlayerController()
{
	return playerController;
}


