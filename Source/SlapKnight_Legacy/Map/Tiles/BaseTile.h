#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTile.generated.h"

class UStaticMeshComponent;
class USceneComponent;
UCLASS()
class SLAPKNIGHT_LEGACY_API ABaseTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseTile();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	USceneComponent* TargetToMove;
	
public:
	UPROPERTY(EditAnywhere)
		FVector pos;
	UPROPERTY(EditAnywhere)
	int tileId;
};
