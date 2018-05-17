// Copyleft

#include "Tile.h"
//#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn,int32 MaxSpawn,float Radius, bool bOnlyRandRotationYaw)
{
	
	//Generating  random quantity of Actors to Spawn
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	//Spawning in location
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		
		FVector SpawnPoint;
		//Spawn If is a empty location.
		bool bFound = FindEmptyLocation(SpawnPoint,Radius);
		if (bFound)
		{
			PlaceActor(ToSpawn, SpawnPoint, bOnlyRandRotationYaw);
		}
			
	}
	
	
}

bool ATile::FindEmptyLocation(FVector& OutLocation,float Radius)
{
	//Max size of our Tile (Box)
	FVector Min(0, -2000, 50);
	FVector Max(4000, 2000, 50);
	//Box where we put random points
	FBox Bounds(Min, Max);
		
	const int32 MaxAttemps=100;
	
	for (size_t i = 0; i < MaxAttemps; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);

		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}
				
	return false;
}


void ATile::PlaceActor(TSubclassOf<AActor>ToSpawn, FVector SpawnPoint,bool bOnlyRandRotationYaw)
{
	
	//UE_LOG(LogTemp,Warning,TEXT("bOnlyRandRotationYaw:%i, NameOfActor:%s"), bOnlyRandRotationYaw,*ToSpawn->GetName())
	if (bOnlyRandRotationYaw)
	{
		
		FRotator SpawnAngle(0, FMath::FRandRange(0, 360), 0);
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPoint, SpawnAngle);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
	else
	{
		FRotator SpawnAngle(0, FMath::FRandRange(0, 360), FMath::FRandRange(0, 360));
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPoint, SpawnAngle);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
}



// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	//CastSphere(GetActorLocation(), 300);
	//CastSphere(GetActorLocation()+FVector(0,0,1000), 300);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	
	//Deberemos convertir las coordenadas de spawn a globales puesto que vamos a Spawnear en el nivel. 
	// ActorToWorld(), GetTransform(),GetActorTransform().
	FVector GlobalLocation=ActorToWorld().TransformPosition(Location);


	//FQuat Identity. 
	//Create a Custom Collision Channel in our Project/Setting/Collision Channel New. 
		//For know which number our engine is going to give to it we must go to our files in the
		// Config file,open DefaultEngine.ini and look for the name which you gave in the editor.
		// You can see the name that the engine assigned to that custom channel and it will be we use in c++.

	
	bool HasHit=GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation,
				FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius)
				);
	//Drawing a DebugSphere (requires include "DrawDebugHelpers.h")
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green ;
	DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100, 0, 3.f);

	//DrawDebugSphere(GetWorld(), Location, Radius, 16, ResultColor, true, 100,0,3.f);

	return !HasHit;
	
}
