// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlapKnight_LegacyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Map/Tiles/BaseTile.h"
#include "Map/Tiles/TileManager.h"

ASlapKnight_LegacyGameModeBase::ASlapKnight_LegacyGameModeBase()
{
	//tileManager = CreateDefaultSubobject<UTileManager>(TEXT("tileManager"));



}

void ASlapKnight_LegacyGameModeBase::BeginPlay()
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
		Cast<ABaseTile>(tile)->tileId = allTiles.Num() - 1;
	}
	playerController = GetWorld()->GetFirstPlayerController();
	playerController->bShowMouseCursor = true;
}


APlayerController* ASlapKnight_LegacyGameModeBase::GetPlayerController()
{
	return playerController;
}


