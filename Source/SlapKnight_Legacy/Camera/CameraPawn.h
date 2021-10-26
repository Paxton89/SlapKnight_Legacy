#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class UCameraComponent;
class ABaseTile;
class ABaseUnit;
class USceneComponent;
class ASlapKnight_LegacyGameModeBase;
class UTileManager;
class USpringArmComponent;
class AHero;
UCLASS()
class SLAPKNIGHT_LEGACY_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();

	//UFunctions
	UFUNCTION(BlueprintImplementableEvent)
	void ClickFailUI();
	UFUNCTION(BlueprintImplementableEvent)
	void HoveringUnitInfo(int tile, int stamina, int health);
	UFUNCTION(BlueprintImplementableEvent)
	void ShowUnitStatsUI(int tile);

	//Functions
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Variables
	UCameraComponent* MainCam;
	USpringArmComponent* SpringArm;
	USceneComponent* Root;
	AHero* HeroUnit;

	bool bHeroMode = false;
	bool CameraLocked;
	int HitTile;
	float CachedMoveSpeed;
	float CamRight;
	float CamForward;
	float RotX;
	float RotZ;
	FVector DefaultPos;
	FQuat DefaultRot;

	ASlapKnight_LegacyGameModeBase* GameMode;
	TArray<AActor*> IgnoreList;
	TArray<int> PairedList;
	TArray<int> neighbours;
	
	//U-Properties
	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	

	//Functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LeftClick();
	void ChangeCameraMode();
	void HeroLookRight(float Value);
	void HeroLookUp(float Value);
	ABaseTile* Tile(int TileId);
	//void DeselectTile();
	void SendUnitToThisTile(ABaseUnit* unit, int newTile, int oldTile);
	void SetAsCurrentSelectedTile(int tile);
	void Rotate(float Value);
	void MouseHoverOverTile(float Value);

protected:


};
