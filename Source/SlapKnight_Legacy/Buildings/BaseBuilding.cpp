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
	//DrawDebugSphere(GetWorld(), AdjacentTiles[0]->GetActorLocation() + AdjacentTiles[0]->GetActorUpVector() * 40, 20, 18, FColor::Orange, true, 1, 0, 2 );
}

void ABaseBuilding::CollectAdjacentTiles()
{
	auto HalfSize = (GetComponentsBoundingBox().GetSize() * 0.5) - 50 ;
	auto WholeSize = GetComponentsBoundingBox().GetSize() * 0.5 + 50;
	TArray<FHitResult> Hits;
	TArray<FHitResult> NeighbourHits;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -100, HalfSize, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, Hits, true, FColor::Yellow);
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -100, WholeSize, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, NeighbourHits, true, FColor::Orange);

	for (auto Tile : Hits) // Find tiles covered by building
	{
		auto CoveredTile = Cast<ABaseTile>(Tile.Actor);
		BuildingTiles.AddUnique(CoveredTile);
	}
	for (auto Tile : NeighbourHits) // Find surrounding tiles 
	{
		auto Neighbour = Cast<ABaseTile>(Tile.Actor);
		if(!BuildingTiles.Contains(Neighbour))
		{
			AdjacentTiles.Add(Neighbour);
		}
	}
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
	}
	if(AdjacentTiles[index]->CurrentUnit != nullptr)
	{
		index++;
		UE_LOG(LogTemp, Warning, TEXT("Tile occupied! trying next tile"));
		SpawnUnit();
	}
	else
	{
		ABaseUnit* NewUnit = Cast<ABaseUnit>(GetWorld()->SpawnActor(SpawnableUnit));
		NewUnit->SetActorLocation(AdjacentTiles[index]->GetActorLocation());
		NewUnit->SetActorRotation(GetActorRotation());
		NewUnit->teamBlue = TeamBlue;
		NewUnit->UpdateMaterial();
		NewUnit->CenterOnTile();
		index = 0;
	}
}

