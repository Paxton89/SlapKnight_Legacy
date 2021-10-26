
#include "SlapKnight_Legacy/Units/BaseUnit.h"
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseUnit::ABaseUnit() // Creates a root and a skeletal mesh.
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Transform"));
	RootComponent = Root;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetCollisionProfileName("NoCollision");
	SkeletalMesh->SetupAttachment(RootComponent);
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Mesh"));
	HeadMesh->SetCollisionProfileName("NoCollision");
	HeadMesh->SetupAttachment(RootComponent);
}


void ABaseUnit::BeginPlay() // Sets this Units basic stats to the max, gives it the correct color and sets it on the tile below.
{
	Super::BeginPlay();
	//CurrentStamina = StaminaMax;
	DefaultRotation = GetActorRotation();
	CurrentHealth = HealthMax;
	IgnoreList.Add(this);
	CenterOnTile();
	UpdateMaterial();

	SkeletalMesh->SetPlayRate(0.5f);
	SkeletalMesh->PlayAnimation(Anim_Idle, true);
	HeadMesh->SetPlayRate(0.5f);
	HeadMesh->PlayAnimation(Anim_Idle, true);
}

void ABaseUnit::Tick(float DeltaTime)// If not in target position, then it moves there, other than that, its empty :D, ifs like this are alright.
{
	Super::Tick(DeltaTime);
	if (CurrentLocation != TargetLocation) Move();
	if (CurrentHealth <= 0) Die();
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
	//CurrentHealth -= damage;
}

void ABaseUnit::Die() // The unit dies.
{
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -50, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	if (Hit.bBlockingHit)
	{
		auto CurrentTile = Cast<ABaseTile>(Hit.Actor);
		CurrentTile->CurrentUnit = nullptr; 
		CurrentTile->DeSelectTile();
		Destroy();
	}
}

void ABaseUnit::CenterOnTile() // Finds a tile underneath to move to and ocuppy.
{
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -100, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	if(Hit.bBlockingHit) //If RayCast hit, Set Unit position to TargetToMove
	{
		auto CurrentTile = Cast<ABaseTile>(Hit.Actor);
		SetActorLocation(CurrentTile->TargetToMove->GetComponentLocation());
		CurrentTile->CurrentUnit = this;
		SkeletalMesh->SetPlayRate(0.5f);
		SkeletalMesh->PlayAnimation(Anim_Idle, true);
		HeadMesh->SetPlayRate(0.5f);
		HeadMesh->PlayAnimation(Anim_Idle, true);
		UE_LOG(LogTemp, Log, TEXT("CENTERED"));
	}
}

void ABaseUnit::SetTargetTile(ABaseTile* TargetTile) // This changes where the unit should be, this perhaps could be a list so it follows the path made and not only the last tile chosen.
{
	TargetLocation = TargetTile->TargetToMove->GetComponentLocation();
	auto LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetTile->GetActorLocation());
	auto LookAtZ = FRotator(GetActorRotation().Pitch,LookAt.Yaw - 90, GetActorRotation().Roll);
	SetActorRotation(LookAtZ);
	SkeletalMesh->SetPlayRate(0.5f);
	SkeletalMesh->PlayAnimation(Anim_Walk, true);
	HeadMesh->SetPlayRate(0.5f);
	HeadMesh->PlayAnimation(Anim_Walk, true);
}

void ABaseUnit::Move() // This lerps the current direction to the desired position, the lerps works perfectly unless we add pathfinding and longer moves, then the speed should be steady.
{
	CurrentLocation = GetActorLocation();
	SetActorLocation(FMath::Lerp(CurrentLocation, TargetLocation,0.04));
	if(FVector::Distance(CurrentLocation, TargetLocation) < 8)
	{
		FHitResult Hit;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * -100, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
		if(Hit.bBlockingHit) TargetLocation = Cast<ABaseTile>(Hit.Actor)->TargetToMove->GetComponentLocation();
		CenterOnTile();
		//SetActorRotation(DefaultRotation);
	}
}

void ABaseUnit::UpdateMaterial() // Updates the material of the units to the one of its team.
{
	if(TeamBlue)
		SkeletalMesh->SetMaterial(0, Mat_BlueTeam);
	else
		SkeletalMesh->SetMaterial(0, Mat_RedTeam);
}

