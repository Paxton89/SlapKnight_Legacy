// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AstarBrain.generated.h"


class ABaseUnit;
class ASlapKnight_LegacyGameModeBase;
class ABaseTile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLAPKNIGHT_LEGACY_API UAstarBrain : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAstarBrain();

	UFUNCTION()
	void UpdateHighlights(TArray<int> finalPath);

	UFUNCTION()
	int CalculateH(int startTile, int endTile);

	UFUNCTION()
	void CalculateG(int startTile, int currentTile);

	UFUNCTION()
	void UpdateATilesStats(int updateTile, int startTile, int endTile);

	UFUNCTION()
	void SetStartTile(int startTile);

	UFUNCTION()
	void SetEndTile(int endTile);

	UFUNCTION()
	void ResetEndStartTile();

	UFUNCTION()
	TArray<int> Calculatepath(int startTile, int endTile);

	UFUNCTION()
	TArray<int> CalculateCorrectPath(int startTile, int endTile);

	UFUNCTION()
	int CalculateFinalCost(TArray<int> path);

	ABaseTile* Tile(int TileId);

protected:

	ASlapKnight_LegacyGameModeBase* GameMode;

public:	


	UPROPERTY()
	int CurrentStart = -1;

	UPROPERTY()
	int CurrentEnd = -1;

	UPROPERTY()
	int CurrentTile = -1;

	UPROPERTY()
	TArray<int> Path;

	UPROPERTY()
	TArray<int> ClosedList;

	UPROPERTY()
	TArray<int> OpenList;

	UPROPERTY()
	TArray<int> CorrectPath;
		
};
