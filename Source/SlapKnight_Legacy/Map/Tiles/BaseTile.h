#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTile.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class ABaseUnit;
class ASlapKnight_LegacyGameModeBase;
class UTileManager;
class UBoxComponent;

UCLASS()
class SLAPKNIGHT_LEGACY_API ABaseTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseTile();
	virtual void Tick(float DeltaTime) override;

	void EstablishTileLegality();
	void IsLegalTile(bool legal);
	bool GetUnitTeam();
	void SelectTile();
	void UpdateNeighbours();
	void ActivateNeighbours(bool activate);
	int GetUnitStamina();
	ABaseTile* Tile(int TileId);
	void RiseTile(int offset);
	UFUNCTION(BlueprintCallable)
	void DeSelectTile();

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeHighlightToRed();
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeHighlightToGreen();
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeHighlightToYellow();

	int TileId;
	int costToMove = 10;
	bool activated;
	bool affordable;
	bool legalTile;
	bool dificultTerrain;
	bool hovered;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool selected;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ABaseUnit* CurrentUnit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector pos;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* TargetToMove;


protected:
	virtual void BeginPlay() override;

	TArray<int> neighbours;

	ASlapKnight_LegacyGameModeBase* gameMode;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* HighlightPlane;

};
