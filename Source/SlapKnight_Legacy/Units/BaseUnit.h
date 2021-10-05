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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentHealth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentStamina;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool teamBlue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int StaminaMax = 30;

	UFUNCTION(BlueprintCallable)
	void NewRoundReset();
	void Die();
	void ReciveDamage(int damage);
	void CenterOnTile();
	void SetTargetTile(ABaseTile* TargetTile);
	void Move();
	void UpdateMaterial();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere)
	UMaterialInstance* Mat_BlueTeam;
	UPROPERTY(EditAnywhere)
	UMaterialInstance* Mat_RedTeam;

	UPROPERTY(EditAnywhere)
	int HealthMax;
	UPROPERTY(EditAnywhere)
	int DamagePower;
	
	bool Moving;
	
	TArray<AActor*> IgnoreList;
	FVector TargetLocation;
	FVector CurrentLocation;

};
