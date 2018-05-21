// Copyleft

#include "InfiniteTerrainGameMode.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	//Setup The ActorPool Component as inheritance in InfinityTerrainGameMode
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName ("Nav MeshVolume Pool"));
	

}



void  AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

	while (VolumeIterator)
	{
		AddToPool(*VolumeIterator);

		++VolumeIterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume *VolumeToAdd)
{
	NavMeshBoundsVolumePool->Add(VolumeToAdd);
	//UE_LOG(LogTemp, Warning, TEXT("Found Volume=%s"), *VolumeToAdd->GetName());
}

