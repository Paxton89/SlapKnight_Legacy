// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SlapKnight_LegacyGameModeBase.generated.h"

//class UTileManager;
class ABaseTile;
UCLASS()
class SLAPKNIGHT_LEGACY_API ASlapKnight_LegacyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	APlayerController* GetPlayerController();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int currentTile;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int DivineValueBlue = 50;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int DivineValueRed = 50;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool TeamBlue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<ABaseTile*> allTiles;
	
protected:
	ASlapKnight_LegacyGameModeBase();

	APlayerController* playerController;

	TArray<AActor*> allActorTiles;

	virtual void BeginPlay() override;

};

