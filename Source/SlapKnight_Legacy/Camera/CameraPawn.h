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
UCLASS()
class SLAPKNIGHT_LEGACY_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();
	UFUNCTION(BlueprintImplementableEvent)
	void ClickFailUI();
	UFUNCTION(BlueprintImplementableEvent)
	void HoveringUnitInfo(int tile, int stamina, int health);
	UFUNCTION(BlueprintImplementableEvent)
	void ShowUnitStatsUI(int tile);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCameraComponent* MainCam;
	USceneComponent* Root;
	int HitTile;
	
	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	float MoveX;
	float MoveY;
	TArray<AActor*> IgnoreList;
	TArray<int> PairedList;
	TArray<int> neighbours;
	
	ASlapKnight_LegacyGameModeBase* gameMode;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LeftClick();
	ABaseTile* Tile(int TileId);
	//void DeselectTile();
	void SendUnitToThisTile(ABaseUnit* unit, int newTile, int oldTile);
	void SetAsCurrentSelectedTile(int tile);
	void Rotate(float Value);
	void MouseHoverOverTile(float Value);

protected:


};
