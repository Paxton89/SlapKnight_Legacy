#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTile.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class ABaseUnit;
class ASlapKnight_LegacyGameModeBase;
UCLASS()
class SLAPKNIGHT_LEGACY_API ABaseTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseTile();
	virtual void Tick(float DeltaTime) override;

	void EstablishTileLegality();

	void GetUnitTeam();

	void GetUnitStamina();

	void SelectTile();

	void DeSelectTile();


	UPROPERTY(EditAnywhere)
	FVector pos;
	
	UPROPERTY(EditAnywhere)
	int tileId;

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
	AActor* CurrentUnit;

	UPROPERTY(EditAnywhere)
	USceneComponent* TargetToMove;

protected:
	virtual void BeginPlay() override;

	ASlapKnight_LegacyGameModeBase* gameMode;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HighlightedPlaneGreen;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HighlightedPlaneYellow;


	

};
