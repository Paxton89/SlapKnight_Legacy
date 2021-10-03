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

	bool GetUnitTeam();

	int GetUnitStamina();

	void SelectTile();

	void DeSelectTile();

	void UpdateNeighbours();

	void ActivateNeighbours(bool activate);


	UPROPERTY(EditAnywhere)
	FVector pos;
	
	UPROPERTY(EditAnywhere)
	int tileId;

	UPROPERTY(EditAnywhere)
	int costToMove = 10;

	UPROPERTY(EditAnywhere)
	bool selected;

	UPROPERTY(EditAnywhere)
	bool activated;

	UPROPERTY(EditAnywhere)
	bool affordable;

	UPROPERTY(EditAnywhere)
	bool legalTile;

	UPROPERTY(EditAnywhere)
	bool dificultTerrain;

	UPROPERTY(EditAnywhere)
	ABaseUnit* CurrentUnit;

	UPROPERTY(EditAnywhere)
	USceneComponent* TargetToMove;

	UPROPERTY(EditAnywhere)
	TArray<ABaseTile*> neighbours;

protected:
	virtual void BeginPlay() override;

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
