//J

#include "SlapKnight_Legacy/Camera/CameraPawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "../SlapKnight_LegacyGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"

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
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), underMouse.ImpactPoint, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, hit, true);
	if (!hit.bBlockingHit) return;
	
	if (hit.GetActor()->IsA(ABaseTile::StaticClass()))
	{
		HitTile = Cast<ABaseTile>(hit.GetActor());

		if (gameMode->currentTile != nullptr) //Deselect CurrentTile if we already have one 
		{
			gameMode->currentTile->DeSelectTile();
		}
		if(HitTile->CurrentUnit != nullptr) // Check if HitTile is holding a Unit
		{	
			HitTile->SelectTile();
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Hit Tile = %f, %f"),HitTile->pos.X,HitTile->pos.Y);
	}

}

