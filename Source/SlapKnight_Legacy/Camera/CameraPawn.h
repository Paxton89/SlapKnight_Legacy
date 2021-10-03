//J

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class UCameraComponent;
class ABaseTile;
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
	void HoveringUnitInfo(ABaseTile* tile, int stamina);
	UFUNCTION(BlueprintImplementableEvent)
	void ShowUnitStatsUI(ABaseTile* tile);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCameraComponent* MainCam;
	USceneComponent* Root;
	ABaseTile* HitTile;
	
	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	float MoveX;
	float MoveY;
	TArray<AActor*> IgnoreList;
	TArray<ABaseTile*> PairedList;
	TArray<ABaseTile*> neighbours;
	
	ASlapKnight_LegacyGameModeBase* gameMode;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LeftClick();
	void Rotate(float Value);
	void MouseHoverOverTile();
	
};
