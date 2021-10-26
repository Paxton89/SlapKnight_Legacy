// J

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Hero.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class ACameraPawn;
class ASlapKnight_LegacyGameModeBase;
UCLASS()
class SLAPKNIGHT_LEGACY_API AHero : public APawn
{
	GENERATED_BODY()

public: //Functions
	AHero();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PlayWalkAnimation(float Value);
	void PlayIdleAnimation();
	void LeftClick();
	void Rotate();
	
	USceneComponent* GetCameraSceneComponent();

	
	UPROPERTY(EditAnywhere)
	float MoveSpeed;

	bool bIdle = true;
	bool bWalking = false;
	bool bLockedIn = false;
	float MouseRoll;
	float MouseYaw;
	
protected:
		//Functions 
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
		//--Variables--//
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	USceneComponent* CameraPos;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* HeadMesh;
	UPROPERTY(EditAnywhere)
	UAnimSequence* Anim_Idle;
	UPROPERTY(EditAnywhere)
	UAnimSequence* Anim_Walk;
	UPROPERTY(EditAnywhere)
	UAnimSequence* Anim_Attack;

	ASlapKnight_LegacyGameModeBase* GameMode;
	ACameraPawn* CameraPawn;
	float ForwardMovement;
	float RightMovement;

	
};
