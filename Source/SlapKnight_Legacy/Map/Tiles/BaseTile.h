#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTile.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class ABaseUnit;
class ASlapKnight_LegacyGameModeBase;
class UBoxComponent;
class UAstarBrain;

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
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeHighlightToBlue();

	//for pathfinding
	int TileId;
	int Parent = -1; //-1 is no parent
	int H;
	int G;
	int F;
	bool DificultTerrain;
	int CostToMove = 10;
	int CombinedCostToMove;
	//for pathfinding

	bool Activated;
	bool Affordable;
	bool LegalTile;
	bool Hovered;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Selected;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ABaseUnit* CurrentUnit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Pos;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* TargetToMove;


	TArray<int> Neighbours;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* HighlightPlane;

protected:
	virtual void BeginPlay() override;

	ASlapKnight_LegacyGameModeBase* GameMode;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;


};
