#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTile.generated.h"

class UStaticMeshComponent;
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
	
};
