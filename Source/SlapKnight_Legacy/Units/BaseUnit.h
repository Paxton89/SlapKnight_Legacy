// J
#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SlapKnight_Legacy/Interfaces/I_Unit.h"
#include "BaseUnit.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class ABaseTile;
UCLASS()
class SLAPKNIGHT_LEGACY_API ABaseUnit : public APawn, public II_Unit
{
	GENERATED_BODY()

public:
	ABaseUnit();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere)
	int CurrentHealth;
	UPROPERTY(EditAnywhere)
	int CurrentStamina;
	UPROPERTY(EditAnywhere)
	bool teamBlue;
	
	void NewRoundReset();
	void Die();
	void CenterOnTile();
	void SetTargetTile(ABaseTile* TargetTile);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	int StaminaMax = 30;
	UPROPERTY(EditAnywhere)
	int HealthMax;
	UPROPERTY(EditAnywhere)
	int DamagePower;
	
	TArray<AActor*> IgnoreList;
	FVector TargetLocation;
	FVector CurrentLocation;

	void UpdateMaterial();
	void Move();
};
