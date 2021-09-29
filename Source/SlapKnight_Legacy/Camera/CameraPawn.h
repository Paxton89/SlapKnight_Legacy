//J

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class UCameraComponent;
class USceneComponent;
class ABaseTile;
UCLASS()
class SLAPKNIGHT_LEGACY_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCameraComponent* MainCam;
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	
	float MoveX;
	float MoveY;
	TArray<AActor*> IgnoreList;
	ABaseTile* HitTile;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LeftClick();
};
