#include "SlapKnight_Legacy/Camera/CameraPawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "../Units/BaseUnit.h"
#include "../SlapKnight_LegacyGameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SlapKnight_Legacy/Buildings/BaseBuilding.h"
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "SlapKnight_Legacy/Units/Hero.h"

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
	HeroUnit = Cast<AHero>(UGameplayStatics::GetActorOfClass(GetWorld(), AHero::StaticClass()));
	DefaultRot = MainCam->GetComponentQuat();
}

void ACameraPawn::Tick(float DeltaTime) // Currently empty, good :D.
{
	Super::Tick(DeltaTime);
	FVector CameraPos = MainCam->GetComponentLocation();
	FQuat CameraRot = MainCam->GetComponentQuat();
	FVector TargetPos;
	FQuat TargetRot;
	if(bHeroMode)
	{
		TargetPos = HeroUnit->GetCameraSceneComponent()->GetComponentLocation();
		TargetRot = HeroUnit->GetCameraSceneComponent()->GetComponentQuat();
		MainCam->SetWorldLocation(FMath::Lerp(CameraPos, TargetPos, 0.02));
		MainCam->SetWorldRotation(FMath::Lerp(CameraRot, TargetRot, 0.01));
	}
	else if(!bHeroMode)
	{
		TargetPos = Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()))->GetComponentLocation();
		TargetPos += Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()))->TargetOffset;
		TargetRot = DefaultRot;
		MainCam->SetWorldLocation(FMath::Lerp(CameraPos, TargetPos, 0.02));
		MainCam->SetWorldRotation(FMath::Lerp(CameraRot, TargetRot, 0.01));
	}
}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

		PlayerInputComponent->BindAxis("MoveForward", this, &ACameraPawn::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ACameraPawn::MoveRight);
		PlayerInputComponent->BindAxis("Rotate", this, &ACameraPawn::Rotate);
		PlayerInputComponent->BindAxis("MouseMoved", this, &ACameraPawn::MouseHoverOverTile);
		PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &ACameraPawn::LeftClick);	
		PlayerInputComponent->BindAction("CameraModeChange", IE_Pressed, this, &ACameraPawn::ChangeCameraMode);
}

void ACameraPawn::MoveForward(float Value) // Move the camera.
{
	if(!bHeroMode)
	{
		MoveY = Value;
		AddActorLocalOffset(FVector(0, MoveY, 0) * MoveSpeed);
	}
}

void ACameraPawn::MoveRight(float Value) // Move the camera.
{
	if(bHeroMode) return;
	
	MoveX = Value;
	AddActorLocalOffset(FVector(MoveX, 0, 0) * MoveSpeed);	
}

void ACameraPawn::Rotate(float Value) // Rotate the camera.
{
	if(bHeroMode) return;
	
	FRotator NewRotation = FRotator(0, Value*2 , 0);
	FQuat QuatRotation = FQuat(NewRotation);
	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);	
}

void ACameraPawn::MouseHoverOverTile(float Value) // Only when the mouse moves, a raycast gets the tile under the mouse and activates the hovering info method.
{
	if(bHeroMode) return;
	
	FHitResult hit;
	gameMode->GetPlayerController()->GetHitResultUnderCursor(ECC_WorldDynamic, false, hit);
	if (hit.Actor != nullptr && hit.Actor->IsA(ABaseTile::StaticClass()) )
	{

		if (Cast<ABaseTile>(hit.Actor) != Tile(HitTile))
		{
			if (Tile(HitTile) != nullptr)
				Tile(HitTile)->RiseTile(-10);
			HitTile = Cast<ABaseTile>(hit.Actor)->TileId;
			Tile(HitTile)->RiseTile(10);
		}
		if (Tile(HitTile) != nullptr)
		{
			//if (Tile(HitTile)->CurrentUnit != nullptr)
			//	HoveringUnitInfo(HitTile, Tile(HitTile)->CurrentUnit->CurrentStamina, Tile(HitTile)->CurrentUnit->CurrentHealth);
		}
	}
}

void ACameraPawn::LeftClick() 
{
	if(bHeroMode) return;
	
	FHitResult UnderMouse;
	gameMode->GetPlayerController()->GetHitResultUnderCursor(ECC_WorldDynamic, false, UnderMouse);
	if(UnderMouse.bBlockingHit && UnderMouse.Actor->IsA(ABaseBuilding::StaticClass())) // Are we clicking a Building?
	{
		auto HitBuilding = Cast<ABaseBuilding>(UnderMouse.Actor);
		if(HitBuilding->TeamBlue == gameMode->teamBlue) // Is the Building on our team?
		{
			HitBuilding->SpawnUnit(); //Spawn Unit
		}
	}
	
	if ( PairedList.Num() > 0 && Tile(HitTile)->legalTile ) // Checks if a tile is selected and if the new tile is legal to move to, if so it sends the unit to the new tile.
	{
		SendUnitToThisTile(Tile(gameMode->currentTile)->CurrentUnit, HitTile, gameMode->currentTile);
		return;
	}
	if (Tile(gameMode->currentTile) != nullptr) // If a tile is currently selected, it deselects it.
		Tile(gameMode->currentTile)->DeSelectTile();
	if (gameMode->allTiles[HitTile] != nullptr && gameMode->allTiles[HitTile]->CurrentUnit != nullptr) // If the clicked tile has a unit, it will select the clicked tile as the new currectly selected tile.
		SetAsCurrentSelectedTile(HitTile);
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Clicked Non-Valid Target"));
	}
}

void ACameraPawn::ChangeCameraMode()
{
	bHeroMode = !bHeroMode;
}

void ACameraPawn::SendUnitToThisTile(ABaseUnit* unit, int newTile, int oldTile)
{
	
	PairedList.Empty(); // Empties List so that a new pair can be selected.
	unit->SetTargetTile(Tile(newTile)); // Tells the unit to change tiles and move there.
	unit->CurrentStamina -= Tile(newTile)->costToMove; // Reduces stamina by tile set amount.
	Tile(newTile)->CurrentUnit = unit; // Adds the unit into the new tile.
	Tile(oldTile)->CurrentUnit = nullptr; // Removes the unit from the old tile.
	Tile(oldTile)->DeSelectTile(); // De-selects old tile
	SetAsCurrentSelectedTile(newTile);// Selects new tile.
}

void ACameraPawn::SetAsCurrentSelectedTile(int tile)
{
	PairedList.Add(tile); // Adds tile to paired list, this can only happen if its empty, ensuring the next click is a neighbour or another unit tile.
	Tile(tile)->SelectTile(); // Tells the tile to change variables to selected.
	ShowUnitStatsUI(tile); // Its suppose to display something or another in UI as a response to having selected a new tile.
}

ABaseTile* ACameraPawn::Tile(int TileId)
{
	return gameMode->allTiles[TileId];
}

