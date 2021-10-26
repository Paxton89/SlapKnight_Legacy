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

	//for pathfinding
	int TileId;
	bool Blocked;
	int Parent = -1; //-1 is no parent
	int H;
	int G;
	int F;
	bool DificultTerrain;
	int DostToMove = 10;
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


protected:
	virtual void BeginPlay() override;

	TArray<int> Neighbours;

	ASlapKnight_LegacyGameModeBase* GameMode;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* HighlightPlane;

};
