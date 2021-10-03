//J

#include "SlapKnight_Legacy/Camera/CameraPawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "../SlapKnight_LegacyGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "SlapKnight_Legacy/Map/Tiles/TileManager.h"
#include "SlapKnight_Legacy/Units/BaseUnit.h"

ACameraPawn::ACameraPawn() // Establish root.
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}


void ACameraPawn::BeginPlay() // Get camera and gamemode for future reference.
{
	Super::BeginPlay();
	gameMode = Cast<ASlapKnight_LegacyGameModeBase>(GetWorld()->GetAuthGameMode());
	MainCam = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
}

void ACameraPawn::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	MouseHoverOverTile();
}


void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACameraPawn::MoveRight);
	PlayerInputComponent->BindAxis("Rotate", this, &ACameraPawn::Rotate);
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &ACameraPawn::LeftClick);
}

void ACameraPawn::MoveForward(float Value)
{
	MoveY = Value;
	AddActorLocalOffset(FVector(MoveX, MoveY, 0) * MoveSpeed);
}

void ACameraPawn::MoveRight(float Value)
{
	MoveX = Value;
	AddActorLocalOffset(FVector(MoveX, MoveY, 0) * MoveSpeed);
}

void ACameraPawn::Rotate(float Value)
{
	FRotator NewRotation = FRotator(0, Value*2 , 0);
	FQuat QuatRotation = FQuat(NewRotation);
	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

void ACameraPawn::MouseHoverOverTile()
{
	FHitResult hit;
	FHitResult underMouse;
	gameMode->GetPlayerController()->GetHitResultUnderCursor(ECC_WorldDynamic, false, underMouse);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), MainCam->GetComponentLocation(), underMouse.ImpactPoint, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::None, hit, true);
	if (!hit.bBlockingHit)
	{
		return;
	}
	if (hit.GetActor()->IsA(ABaseTile::StaticClass()))
	{
		ABaseTile* hoveringTile = Cast<ABaseTile>(hit.GetActor());
		if (hoveringTile->CurrentUnit != nullptr) // if HitTile has a unit, Select this tile
		{
			
			HoveringUnitInfo(hoveringTile, hoveringTile->CurrentUnit->CurrentStamina);
		}
	}
}

void ACameraPawn::LeftClick()
{
	FHitResult hit;
	FHitResult underMouse;
	gameMode->GetPlayerController()->GetHitResultUnderCursor(ECC_WorldDynamic, false, underMouse);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), MainCam->GetComponentLocation(), underMouse.ImpactPoint, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, hit, true);

	bool hitSomething = underMouse.bBlockingHit;
	if (!hit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("hit nothing "));
		ClickFailUI();//shows troll face on ui for a fraction of a second
		return;
	}
	
	if (hit.GetActor()->IsA(ABaseTile::StaticClass()))
	{
		HitTile = Cast<ABaseTile>(hit.GetActor());
		if(PairedList.Num() > 0)
		{
			if (HitTile->CurrentUnit == nullptr && HitTile->legalTile) // if HitTile has a unit, Select this tile
			{
				gameMode->currentTile->CurrentUnit->SetTargetTile(HitTile);
				gameMode->currentTile->CurrentUnit->CurrentStamina -= HitTile->costToMove;
				gameMode->currentTile->DeSelectTile();
				HitTile->CurrentUnit = gameMode->currentTile->CurrentUnit;
				gameMode->currentTile->CurrentUnit = nullptr;
				PairedList.Empty();
				return;
			}

		}
		if (gameMode->currentTile != nullptr) // if CurrentTile has a value
		{
			gameMode->currentTile->DeSelectTile();
			
		}
		if(HitTile->CurrentUnit != nullptr) // if HitTile has a unit, Select this tile
		{
			PairedList.Add(HitTile);
			HitTile->SelectTile();
			ShowUnitStatsUI(HitTile);
		}	

		UE_LOG(LogTemp, Warning, TEXT("Hit Tile id = %d"),HitTile->tileId);
	}

}



