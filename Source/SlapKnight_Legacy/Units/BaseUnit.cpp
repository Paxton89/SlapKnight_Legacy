
#include "SlapKnight_Legacy/Units/BaseUnit.h"
#include <stdbool.h>
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseUnit::ABaseUnit() // Creates a root and a skeletal mesh.
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Transform"));
	RootComponent = Root;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetCollisionProfileName("NoCollision");
	SkeletalMesh->SetupAttachment(RootComponent);
}


void ABaseUnit::BeginPlay() // Sets this Units basic stats to the max, gives it the correct color and sets it on the tile below.
{
	Super::BeginPlay();
	CurrentStamina = StaminaMax;
	CurrentHealth = HealthMax;
	IgnoreList.Add(this);
	CenterOnTile();
	UpdateMaterial();
}

void ABaseUnit::Tick(float DeltaTime)// If not in target position, then it moves there, other than that, its empty :D, ifs like this are alright.
{
	Super::Tick(DeltaTime);
	if (CurrentLocation != TargetLocation ) Move();
}

void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseUnit::NewRoundReset() // This resets the stamina of this unit at the start of its teams turn.
{
	CurrentStamina = StaminaMax;
}

void ABaseUnit::ReciveDamage(int damage) // The units health decreases by the amount of damage and checks if it should die or not by the damage.
{
	CurrentHealth -= damage;
	if (CurrentHealth <= 0) Die();
}

void ABaseUnit::Die() // The unit dies.
{
	UE_LOG(LogTemp, Log, TEXT("Health is below 0 -- Dying"));
}

void ABaseUnit::CenterOnTile() // Finds a tile underneath to move to and ocuppy.
{
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -50, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	if(Hit.bBlockingHit)
	{
		auto CurrentTile = Cast<ABaseTile>(Hit.Actor);
		SetActorLocation(CurrentTile->TargetToMove->GetComponentLocation());
		CurrentTile->CurrentUnit = this;
	}
}

void ABaseUnit::SetTargetTile(ABaseTile* TargetTile) // This changes where the unit should be, this perhaps could be a list so it follows the path made and not only the last tile chosen.
{
	TargetLocation = TargetTile->TargetToMove->GetComponentLocation();
}

void ABaseUnit::Move() // This lerps the current direction to the desired position, the lerps works perfectly unless we add pathfinding and longer moves, then the speed should be steady.
{
	CurrentLocation = GetActorLocation();
	SetActorLocation(FMath::Lerp(CurrentLocation, TargetLocation,0.03));
	if(CurrentLocation == TargetLocation)
	{
		CenterOnTile();
		UE_LOG(LogTemp, Log, TEXT("Location Reached"));
	}
}

void ABaseUnit::UpdateMaterial() // Updates the material of the units to the one of its team.
{
	if(teamBlue)
		SkeletalMesh->SetMaterial(0, Mat_BlueTeam);
	else
		SkeletalMesh->SetMaterial(0, Mat_RedTeam);
}

