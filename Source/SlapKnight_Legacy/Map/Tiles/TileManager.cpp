#include "TileManager.h"
#include "Kismet/GameplayStatics.h"

UTileManager::UTileManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTileManager::BeginPlay()
{
	Super::BeginPlay();


}

void UTileManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

