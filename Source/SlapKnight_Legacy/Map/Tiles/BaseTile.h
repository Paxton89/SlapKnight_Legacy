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


	UPROPERTY(EditAnywhere)
	FVector pos;
	
	UPROPERTY(EditAnywhere)
	int tileId;

	UPROPERTY(EditAnywhere)
	bool activated;

	UPROPERTY(EditAnywhere)
	bool affordable;

	UPROPERTY(EditAnywhere)
	AActor* occupied;

	UPROPERTY(EditAnywhere)
	USceneComponent* TargetToMove;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	

};
