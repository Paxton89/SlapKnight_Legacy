// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlapKnight_LegacyGameModeBase.h"
#include "GameFramework/Actor.h"
#include "ActorComponents/AstarBrain.h"
#include "Camera/CameraPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Map/Tiles/BaseTile.h"

ASlapKnight_LegacyGameModeBase::ASlapKnight_LegacyGameModeBase()
{
}

void ASlapKnight_LegacyGameModeBase::BeginPlay() // This makes a list of all all already casted tiles and assignes them a position.
{
	Super::BeginPlay();
	SetUpAllTiles();
	SetUpAstarBrain();


}

APlayerController* ASlapKnight_LegacyGameModeBase::GetPlayerController()
{
	return playerController;
}

UAstarBrain* ASlapKnight_LegacyGameModeBase::GetAstarBrain()
{

	return AstarBrain;
}

ACameraPawn* ASlapKnight_LegacyGameModeBase::GetCameraPawn()
{
	return CameraPawn;
}

void ASlapKnight_LegacyGameModeBase::SetUpAstarBrain()
{
	TArray<UAstarBrain*> Comps;
	GetComponents(Comps);
	if (Comps.Num() > 0)
	{
		UAstarBrain* FoundComp = Comps[0];
		AstarBrain = FoundComp;
	}
}

void ASlapKnight_LegacyGameModeBase::SetUpAllTiles()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseTile::StaticClass(), allActorTiles);
	for (auto tile : allActorTiles)
	{
		int x = tile->GetActorLocation().X / 100;
		int y = tile->GetActorLocation().Y / 100;
		Cast<ABaseTile>(tile)->Pos.X = -x;
		Cast<ABaseTile>(tile)->Pos.Y = -y;
		allTiles.Add(Cast<ABaseTile>(tile));
		Cast<ABaseTile>(tile)->TileId = allTiles.Num() - 1;
	}
	playerController = GetWorld()->GetFirstPlayerController();
	playerController->bShowMouseCursor = true;
	CameraPawn = Cast<ACameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraPawn::StaticClass()));
}


