//J

#include "SlapKnight_Legacy/Camera/CameraPawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SlapKnight_Legacy/Map/Tiles/BaseTile.h"

ACameraPawn::ACameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	MainCam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MainCam->SetupAttachment(RootComponent);
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldOffset(FVector(MoveX, MoveY, 0) * MoveSpeed);
}


void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACameraPawn::MoveRight);
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

void ACameraPawn::LeftClick()
{
	FHitResult Hit;
	ABaseTile* HitTile;;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 10000, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	HitTile = Cast<ABaseTile>(Hit.Actor);
}

