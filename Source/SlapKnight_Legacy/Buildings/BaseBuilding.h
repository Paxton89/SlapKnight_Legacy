// J

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBuilding.generated.h"

class ABaseUnit;
class ABaseTile;
UCLASS()
class SLAPKNIGHT_LEGACY_API ABaseBuilding : public AActor
{
	GENERATED_BODY()
	
public:	//Variables
	ABaseBuilding();

	UPROPERTY(EditAnywhere)
	int BuildCost;
	UPROPERTY(EditAnywhere)
	bool TeamBlue = false;

	TArray<ABaseTile*> BuildingTiles;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseUnit> SpawnableUnit;

	int index = 0;
	TArray<AActor*> IgnoreList;
	TArray<ABaseTile*> AdjacentTiles;
	void CollectAdjacentTiles();

public:	//Functions
	virtual void Tick(float DeltaTime) override;
	void SpawnUnit();
};
