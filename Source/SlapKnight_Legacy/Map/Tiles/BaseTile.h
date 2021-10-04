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

	ABaseUnit* CurrentUnit;
	
	UPROPERTY(EditAnywhere)
	FVector pos;
	UPROPERTY(EditAnywhere)
	USceneComponent* TargetToMove;


protected:
	virtual void BeginPlay() override;

	TArray<ABaseTile*> neighbours;
	ASlapKnight_LegacyGameModeBase* gameMode;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HighlightedPlaneGreen;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HighlightedPlaneYellow;

};
