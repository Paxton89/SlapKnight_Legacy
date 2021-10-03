// J
#include "SlapKnight_Legacy/Units/BaseUnit.h"

#include <stdbool.h>

#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseUnit::ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Transform"));
	RootComponent = Root;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetCollisionProfileName("NoCollision");
	SkeletalMesh->SetupAttachment(RootComponent);
	
}


void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	CurrentStamina = StaminaMax;
	CurrentHealth = HealthMax;
	IgnoreList.Add(this);
	CenterOnTile();
	UpdateMaterial();
}

void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(CurrentHealth <= 0) Die();
	if(CurrentLocation != TargetLocation) Move();
}

void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseUnit::NewRoundReset()
{
	CurrentStamina = StaminaMax;
}

void ABaseUnit::Die()
{
	UE_LOG(LogTemp, Log, TEXT("Health is below 0 -- Dying"));
}

void ABaseUnit::CenterOnTile()
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

void ABaseUnit::SetTargetTile(ABaseTile* TargetTile)
{
	TargetLocation = TargetTile->TargetToMove->GetComponentLocation();
}

void ABaseUnit::Move()
{
	CurrentLocation = GetActorLocation();
	SetActorLocation(FMath::Lerp(CurrentLocation, TargetLocation,0.03));
	if(CurrentLocation == TargetLocation)
	{
		CenterOnTile();
		UE_LOG(LogTemp, Log, TEXT("Location Reached"));
	}
}

void ABaseUnit::UpdateMaterial()
{
	if(teamBlue)
	{
		SkeletalMesh->SetMaterial(0, Mat_BlueTeam);
	}
	else
	{
		SkeletalMesh->SetMaterial(0, Mat_RedTeam);
	}
}

