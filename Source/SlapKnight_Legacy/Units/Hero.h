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
	void LeftClick();
	void Rotate(float Value);
	
	USceneComponent* GetCameraSceneComponent(); 
public:	//Variables

	
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

	ASlapKnight_LegacyGameModeBase* GameMode;
	ACameraPawn* CameraPawn;

	
};
