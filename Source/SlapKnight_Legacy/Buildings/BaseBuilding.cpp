// J


#include "SlapKnight_Legacy//Buildings/BaseBuilding.h"

#include "DrawDebugHelpers.h"
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SlapKnight_Legacy/Units/BaseUnit.h"

ABaseBuilding::ABaseBuilding()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = Mesh;
}

void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();
	IgnoreList.Add(this);
	CollectAdjacentTiles();
}

void ABaseBuilding::CollectAdjacentTiles()
{
	auto HalfSize = (GetComponentsBoundingBox().GetSize() * 0.5) - 50; // - Half the size of a tile
	auto WholeSize = GetComponentsBoundingBox().GetSize() * 0.5 + 50; // + Half the size of a tile
	TArray<FHitResult> Hits;
	TArray<FHitResult> NeighbourHits;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -100, HalfSize, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, Hits, true, FColor::Yellow);
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -100, WholeSize, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, NeighbourHits, true, FColor::Orange);

	for (auto Tile : Hits)
	{
		auto CoveredTile = Cast<ABaseTile>(Tile.Actor);
		BuildingTiles.AddUnique(CoveredTile);
	} // Find tiles covered by building
	for (auto Tile : NeighbourHits)
	{
		auto Neighbour = Cast<ABaseTile>(Tile.Actor);
		if(!BuildingTiles.Contains(Neighbour))
		{
			AdjacentTiles.Add(Neighbour);
		}
	} // Find surrounding tiles
	
	UE_LOG(LogTemp, Warning, TEXT("Building covers %i Tiles"), BuildingTiles.Num());
	UE_LOG(LogTemp, Warning, TEXT("AdjacentTiles %i"), AdjacentTiles.Num());
}

void ABaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseBuilding::SpawnUnit()
{
	if(index >= AdjacentTiles.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("NO FREE TILES TO SPAWN UNIT!"));
		index = 0;
		return;
	} //Check if index is bigger than number of availible tiles, reset index & return
	if(AdjacentTiles[index]->CurrentUnit != nullptr)
	{
		index++;
		UE_LOG(LogTemp, Log, TEXT("Tile occupied! trying next tile"));
		SpawnUnit();
	} //If Checked tile is not free, increase index and run function again
	else
	{
		ABaseUnit* NewUnit = Cast<ABaseUnit>(GetWorld()->SpawnActor(SpawnableUnit));
		NewUnit->SetActorLocation(AdjacentTiles[index]->GetActorLocation());
		NewUnit->SetActorRotation(GetActorRotation());
		NewUnit->teamBlue = TeamBlue;
		NewUnit->UpdateMaterial();
		NewUnit->CenterOnTile();
		index = 0;
	} // Spawn Unit, Reset Index.
}

