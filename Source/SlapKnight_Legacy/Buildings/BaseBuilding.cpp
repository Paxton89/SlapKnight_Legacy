// J


#include "SlapKnight_Legacy//Buildings/BaseBuilding.h"
#include "DrawDebugHelpers.h"
#include "SlapKnight_Legacy/SlapKnight_LegacyGameModeBase.h"
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SlapKnight_Legacy/Units/BaseUnit.h"

ABaseBuilding::ABaseBuilding()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName("BlockAllDynamic");
	Mesh->SetupAttachment(RootComponent);
}

void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<ASlapKnight_LegacyGameModeBase>(GetWorld()->GetAuthGameMode());
	IgnoreList.Add(this);
	CollectAdjacentTiles();
}

void ABaseBuilding::CollectAdjacentTiles()
{
	auto InnerBox = (GetComponentsBoundingBox().GetSize() * 0.5) - 50; // Will capture the tiles under the building
	auto OuterBox = GetComponentsBoundingBox().GetSize() * 0.2 + 50; //Will capture the tiles adjacent to the building
	TArray<FHitResult> InnerBoxHits;
	TArray<FHitResult> OuterBoxHits;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -100, InnerBox, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, InnerBoxHits, true, FColor::Yellow);
	if(Mesh->GetRelativeRotation().Yaw == 0) // Is mesh not rotated? Then this is the correct TraceCast
	{
		UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Mesh->GetComponentLocation() + Mesh->GetRightVector() * 100, GetActorLocation() + GetActorUpVector() * -100, OuterBox, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, OuterBoxHits, true, FColor::Orange);
	}
	else// Is mesh rotated? Then this is the correct TraceCast
	{
		UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Mesh->GetComponentLocation() + Mesh->GetForwardVector() * -100, GetActorLocation() + GetActorUpVector() * -100, OuterBox, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, OuterBoxHits, true, FColor::Orange);

	}
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
		if((TeamBlue && GameMode->DivineValueBlue >= NewUnit->CostToSpawn) || (!TeamBlue && GameMode->DivineValueRed >= NewUnit->CostToSpawn)) // Make sure player can afford to spawn new unit
		{
			NewUnit->SetActorLocation(AdjacentTiles[index]->TargetToMove->GetComponentLocation());
			NewUnit->SetActorRotation(GetActorForwardVector().ToOrientationRotator());
			NewUnit->TeamBlue = TeamBlue;
			NewUnit->UpdateMaterial();
			NewUnit->CenterOnTile();
			NewUnit->DefaultRotation = NewUnit->GetActorRotation();
			if(TeamBlue) GameMode->DivineValueBlue -= NewUnit->CostToSpawn;
			else GameMode->DivineValueRed -= NewUnit->CostToSpawn;	
		}
		else // player does not have enough resource
		{
			NewUnit->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Not Enough Recource To Spawn Unit"));
		}
		index = 0;
	}
}

