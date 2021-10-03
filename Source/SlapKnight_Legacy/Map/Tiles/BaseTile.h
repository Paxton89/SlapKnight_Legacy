#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTile.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class ABaseUnit;
class ASlapKnight_LegacyGameModeBase;
class UTileManager;

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
	void DeSelectTile();
	void UpdateNeighbours();
	void ActivateNeighbours(bool activate);
	int GetUnitStamina();

	int TileId;
	int costToMove = 10;
	bool selected;
	bool activated;
	bool affordable;
	bool legalTile;
	bool dificultTerrain;

	FVector pos;
	ABaseUnit* CurrentUnit;
	USceneComponent* TargetToMove;

protected:
	virtual void BeginPlay() override;

	TArray<ABaseTile*> neighbours;
	ASlapKnight_LegacyGameModeBase* gameMode;
	UStaticMeshComponent* Mesh;
	UStaticMeshComponent* HighlightedPlaneGreen;
	UStaticMeshComponent* HighlightedPlaneYellow;

};
