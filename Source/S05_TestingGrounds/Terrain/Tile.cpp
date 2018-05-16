// Copyleft

#include "Tile.h"
//#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn,int32 MaxSpawn,bool bOnlyRandRotationYaw)
{

	FVector Min(0, -2000, 50);
	FVector Max(4000, 2000, 50);
	//Box where we put random points
	FBox Bounds(Min, Max);
	
	//Generating  random quantity of Actors to Spawn
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	//Spawning in location
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint=	FMath::RandPointInBox(Bounds);
		
		//Giving random rotation
		if (bOnlyRandRotationYaw)
		{
			FRotator SpawnAngle(0, FMath::FRandRange(0, 360), 0);
			AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPoint, SpawnAngle);
			Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		}
		else
		{
			FRotator SpawnAngle(0, FMath::FRandRange(0, 360),FMath::FRandRange(0, 360));
			AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPoint, SpawnAngle);
			Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		}
			
		//Spawned->SetActorRelativeLocation(SpawnPoint);
		
		UE_LOG(LogTemp, Warning, TEXT("spawn point: %s, Number of Assets Generated:%i"), *SpawnPoint.ToCompactString(),NumberToSpawn);
	
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
