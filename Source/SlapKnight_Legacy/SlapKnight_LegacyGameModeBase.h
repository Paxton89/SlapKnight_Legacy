// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SlapKnight_LegacyGameModeBase.generated.h"

class UTileManager;
class ABaseTile;
UCLASS()
class SLAPKNIGHT_LEGACY_API ASlapKnight_LegacyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	UTileManager* GetTileManager();

	APlayerController* GetPlayerController();

	UPROPERTY(VisibleAnywhere)
	ABaseTile* currentTile;


protected:
	ASlapKnight_LegacyGameModeBase();

	APlayerController* playerController;

	virtual void BeginPlay() override;


	UTileManager* tileManager;


};
