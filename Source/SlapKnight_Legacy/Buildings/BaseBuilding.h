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
	TSubclassOf<ABaseUnit> Unit;
	
protected:
	virtual void BeginPlay() override;
	TArray<ABaseTile*> AdjacentTiles;
	

public:	//Functions
	virtual void Tick(float DeltaTime) override;
	void SpawnUnit();
};
