// J

#include "SlapKnight_Legacy/Units/Hero.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "SlapKnight_Legacy/SlapKnight_LegacyGameModeBase.h"
#include "SlapKnight_Legacy/Camera/CameraPawn.h"

AHero::AHero()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	CameraPos = CreateDefaultSubobject<USceneComponent>(TEXT("Camera Position"));
	CameraPos->SetupAttachment(RootComponent);
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetCollisionProfileName("NoCollision");
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HeadMesh");
	HeadMesh->SetupAttachment(RootComponent);
	HeadMesh->SetCollisionProfileName("NoCollision");
}

void AHero::MoveForward(float Value)
{
}

void AHero::LeftClick()
{
}

void AHero::Rotate(float Value)
{
}

void AHero::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<ASlapKnight_LegacyGameModeBase>(GetWorld()->GetAuthGameMode());
	CameraPawn = GameMode->GetCameraPawn();
	BodyMesh->SetPlayRate(0.5f);
	BodyMesh->PlayAnimation(Anim_Idle, true);
	HeadMesh->SetPlayRate(0.5f);
	HeadMesh->PlayAnimation(Anim_Idle, true);
}

void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AHero::LeftClick);
	PlayerInputComponent->BindAxis("MouseMoved", this, &AHero::Rotate);
}

USceneComponent* AHero::GetCameraSceneComponent()
{
	return CameraPos;
}

