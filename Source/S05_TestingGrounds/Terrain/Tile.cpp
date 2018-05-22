// Copyleft

#include "Tile.h"
//#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActorPool.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Settings Defaults Value
	MinExtents=FVector (0, -2000, 50);
	MaxExtents=FVector (4000, 2000, 50);

}


//Setter Method
void ATile::SetPool(UActorPool* InPool)
{
	if (!InPool) { return; }
	
	Pool = InPool;
	UE_LOG(LogTemp, Warning, TEXT("[%s]Setting Pool %s"), *GetName(), *InPool->GetName());
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	
	NavMeshBoundsVolume = Pool->CheckOut();
	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]Not Enough Actors In Pool."), *GetName());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s]CHECKOUT: { %s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	
		//Using the Tile Location as the location for out NavMeshBoundsVolume
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation());
}


 void ATile::Tick(float DeltaTime)
{

}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	

}


void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	
	Pool->Return(NavMeshBoundsVolume);

}


void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn , int MaxSpawn , float Radius , float MinScale , float MaxScale , bool bOnlyRandRotationYaw)
{
	//Generating  random quantity of Actors to Spawn
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	//Spawning in location
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		
		FVector SpawnPoint;
		//Spawn If is a empty location.
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		bool bFound = FindEmptyLocation(SpawnPoint,Radius*RandomScale);
		

		if (bFound)
		{
			float RandomRotation = FMath::RandRange(-180.f, 180.f);
			
			
			PlaceActor(ToSpawn, SpawnPoint, bOnlyRandRotationYaw, RandomRotation,RandomScale);
		}
			
	}
	
	
}

bool ATile::FindEmptyLocation(FVector& OutLocation,float Radius)
{
	
	//Box where we put random points
	FBox Bounds(MinExtents, MaxExtents);
		
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


void ATile::PlaceActor(TSubclassOf<AActor>ToSpawn, FVector SpawnPoint,bool bOnlyRandRotationYaw,float RandomRotation, float RandomScale)
{
	

	//Was my solution
	//UE_LOG(LogTemp,Warning,TEXT("bOnlyRandRotationYaw:%i, NameOfActor:%s"), bOnlyRandRotationYaw,*ToSpawn->GetName())
	/*if (bOnlyRandRotationYaw)
	{
		
		FRotator SpawnAngle(0, FMath::FRandRange(0, 360), 0);
		
	}
	else
	{
		FRotator SpawnAngle(0, FMath::FRandRange(0, 360), FMath::FRandRange(0, 360));
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPoint, SpawnAngle);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}*/

	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorScale3D(FVector(RandomScale));

	if (bOnlyRandRotationYaw) 
	{
		Spawned->SetActorRotation(FRotator(0, RandomRotation, 0));
	}
	
	else
	{
		Spawned->SetActorRotation(FRotator(0, RandomRotation, RandomRotation));
	}

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
	//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100, 0, 3.f);

	//DrawDebugSphere(GetWorld(), Location, Radius, 16, ResultColor, true, 100,0,3.f);

	return !HasHit;
	
}
