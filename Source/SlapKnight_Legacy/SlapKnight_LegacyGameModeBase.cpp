// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlapKnight_LegacyGameModeBase.h"
#include "Map/Tiles/TileManager.h"

ASlapKnight_LegacyGameModeBase::ASlapKnight_LegacyGameModeBase()
{
	tileManager = CreateDefaultSubobject<UTileManager>(TEXT("tileManager"));



}

void ASlapKnight_LegacyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	playerController = GetWorld()->GetFirstPlayerController();
	playerController->bShowMouseCursor = true;
}


UTileManager* ASlapKnight_LegacyGameModeBase::GetTileManager()
{
	return tileManager;
}


