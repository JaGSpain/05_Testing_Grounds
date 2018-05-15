// Copyleft

#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



void ATile::PlaceActors()
{

	FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);
	//Box where we put random points
	FBox Bounds(Min, Max);
	
	//Generating  random points
	for (size_t i = 0; i < 20; i++)
	{
	FVector SpawnPoint=	FMath::RandPointInBox(Bounds);
	UE_LOG(LogTemp, Warning, TEXT("spawn point: %s"), *SpawnPoint.ToCompactString());
	
	}
	
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
