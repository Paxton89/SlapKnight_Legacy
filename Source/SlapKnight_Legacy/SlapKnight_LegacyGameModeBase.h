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

	ABaseTile* currentTile;


protected:
	ASlapKnight_LegacyGameModeBase();

	virtual void BeginPlay() override;

	APlayerController* playerController;

	UTileManager* tileManager;


};
