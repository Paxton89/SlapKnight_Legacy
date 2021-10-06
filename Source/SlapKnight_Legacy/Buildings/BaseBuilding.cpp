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
	auto InnerBox = (GetComponentsBoundingBox().GetSize() * 0.5) - 50; // Will capture the tiles under the building
	auto OuterBox = GetComponentsBoundingBox().GetSize() * 0.5 + 50; //Will capture the tiles adjacent to the building
	TArray<FHitResult> InnerBoxHits;
	TArray<FHitResult> OuterBoxHits;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -100, InnerBox, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, InnerBoxHits, true, FColor::Yellow);
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -100, OuterBox, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, OuterBoxHits, true, FColor::Orange);

	for (auto HitByInner : InnerBoxHits) // Adds Tiles hit by InnerBox to an Array.
	{
		auto CurrentTile = Cast<ABaseTile>(HitByInner.Actor);
		BuildingTiles.AddUnique(CurrentTile);
	}
	for (auto HitByOuter : OuterBoxHits) // // Adds Tiles hit by OuterBox to an Array. (unless they are directly under the building)
	{
		auto CurrentTile = Cast<ABaseTile>(HitByOuter.Actor);
		if(!BuildingTiles.Contains(CurrentTile))
		{
			AdjacentTiles.Add(CurrentTile);
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
	if(index >= AdjacentTiles.Num()) //Index is bigger than available tiles, No more room to spawn units. Reset Index & Return
	{
		UE_LOG(LogTemp, Warning, TEXT("No free tiles! Cannot spawn unit"));
		index = 0;
		return;
	}
	if(AdjacentTiles[index]->CurrentUnit != nullptr) //Checked if tile already has a Unit. If it does, increase index and run function again.
	{
		index++;
		SpawnUnit();
	}
	else // There is a free tile! Spawn Unit. Reset Index.
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

