//J

#include "SlapKnight_Legacy/Camera/CameraPawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "../SlapKnight_LegacyGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"
#include "SlapKnight_Legacy/Units/BaseUnit.h"

ACameraPawn::ACameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	//MainCam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//MainCam->SetupAttachment(RootComponent);
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	gameMode = Cast<ASlapKnight_LegacyGameModeBase>(GetWorld()->GetAuthGameMode());
	MainCam = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
}

void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector(MoveX, MoveY, 0) * MoveSpeed);
	//AddActorWorldOffset(FVector(MoveX, MoveY, 0) * MoveSpeed);
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
}

void ACameraPawn::MoveRight(float Value)
{
	MoveX = Value;
}

void ACameraPawn::Rotate(float Value)
{

	FRotator NewRotation = FRotator(0, Value , 0);

	FQuat QuatRotation = FQuat(NewRotation);

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);


}

void ACameraPawn::LeftClick()
{
	FHitResult hit;
	FHitResult underMouse;
	gameMode->GetPlayerController()->GetHitResultUnderCursor(ECC_WorldDynamic, false, underMouse);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), MainCam->GetComponentLocation(), underMouse.ImpactPoint, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::Persistent, hit, true);
	if (!hit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraPawn hit nothing"));
		return;
	}
	
	if (hit.GetActor()->IsA(ABaseTile::StaticClass()))
	{
		HitTile = Cast<ABaseTile>(hit.GetActor());
		if(PairedList.Num() > 0)
		{
			gameMode->currentTile->CurrentUnit->SetTargetTile(HitTile);
			HitTile->CurrentUnit = gameMode->currentTile->CurrentUnit;
			gameMode->currentTile->CurrentUnit = nullptr;
			gameMode->currentTile->selected = false;
			PairedList.Empty();
			return;
		}
		if (gameMode->currentTile != nullptr) // if CurrentTile has a value
		{
			gameMode->currentTile->DeSelectTile();
		}
		if(HitTile->CurrentUnit != nullptr) // if HitTile has a unit, Select this tile
		{
			PairedList.Add(HitTile);
			HitTile->SelectTile();
		}	

		UE_LOG(LogTemp, Warning, TEXT("Hit Tile = %f, %f"),HitTile->pos.X,HitTile->pos.Y);
	}

}

