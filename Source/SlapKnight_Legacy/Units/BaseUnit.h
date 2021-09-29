// J
#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SlapKnight_Legacy/Interfaces/I_Unit.h"
#include "BaseUnit.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SLAPKNIGHT_LEGACY_API ABaseUnit : public APawn, public II_Unit
{
	GENERATED_BODY()

public:
	ABaseUnit();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UFUNCTION()
    virtual void MoveForward() override;
};
