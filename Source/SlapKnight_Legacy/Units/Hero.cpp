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
	Rotate();
}

void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	/*
	PlayerInputComponent->BindAxis("MoveForward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHero::MoveRight);
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AHero::LeftClick);
	PlayerInputComponent->BindAxis("MouseMoved", this, &AHero::Rotate);
	*/
}

void AHero::MoveForward(float Value)
{
	ForwardMovement = Value;
	AddActorLocalOffset(FVector(ForwardMovement, 0, 0) * MoveSpeed);
	PlayWalkAnimation(Value);
}

void AHero::MoveRight(float Value)
{
	RightMovement = -Value;
	AddActorLocalOffset(FVector(0, RightMovement, 0) * MoveSpeed);
}

void AHero::LeftClick()
{
	if(!bLockedIn || bWalking) return;
	BodyMesh->SetPlayRate(0.5f);
	BodyMesh->PlayAnimation(Anim_Attack, false);
	HeadMesh->SetPlayRate(0.5f);
	HeadMesh->PlayAnimation(Anim_Attack, false);
}

void AHero::Rotate()
{
	if(!bLockedIn) return;
	FRotator NewRot = GetActorRotation();
	NewRot.Yaw += MouseYaw;
	SetActorRotation(NewRot);
}

auto CoolInt = 0;
void AHero::PlayWalkAnimation(float Value)
{
	if(CoolInt == 0 && Value > 0 && bIdle)
	{
		bWalking = true;
		UE_LOG(LogTemp,Log, TEXT("Started Walking"));
		BodyMesh->SetPlayRate(0.5f);
		BodyMesh->PlayAnimation(Anim_Walk, true);
		HeadMesh->SetPlayRate(0.5f);
		HeadMesh->PlayAnimation(Anim_Walk, true);
		CoolInt++;
	}
	else if(CoolInt > 0 && Value == 0 && bWalking)
	{
		UE_LOG(LogTemp,Log, TEXT("Stopped Walking"));
		bWalking = false;
		bIdle = true;
		CoolInt = 0;
		PlayIdleAnimation();
	}
}

void AHero::PlayIdleAnimation()
{
	BodyMesh->SetPlayRate(0.5f);
	BodyMesh->PlayAnimation(Anim_Idle, true);
	HeadMesh->SetPlayRate(0.5f);
	HeadMesh->PlayAnimation(Anim_Idle, true);
}

USceneComponent* AHero::GetCameraSceneComponent()
{
	return CameraPos;
}

