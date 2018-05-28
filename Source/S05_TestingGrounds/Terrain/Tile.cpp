// Copyleft

#include "Tile.h"
//#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Settings Defaults Value
	MinExtents=FVector (0, -2000, 50);
	MaxExtents=FVector (4000, 2000, 50);
	
	NavigationBoundsOffSet = FVector(2000, 0, 0);
	

}


//Setter Method
void ATile::SetPool(UActorPool* InPool)
{
		
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

	UE_LOG(LogTemp, Warning, TEXT("[%s]Checked out: { %s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	
	//Using the Tile Location as the location for out NavMeshBoundsVolume
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation()+NavigationBoundsOffSet	);

	//Updating The UNavMeshVolume
	GetWorld()->GetNavigationSystem()->Build();
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
	
	if (!Pool && !NavMeshBoundsVolume)
	{
		return;
	}
	Pool->Return(NavMeshBoundsVolume);
}


//**Placing Actors**//
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn , int MaxSpawn ,float Radius,float MinScale,float MaxScale,bool bOnlyRandRotationYaw)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale, false);
			
}


//**Placing AI**// 
void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1, true);

}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale, bool bOnlyRandRotationYaw)
{
	
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	//Spawning in location
	for (size_t i = 0; i < NumberToSpawn; i++)
	{

		FSpawnPosition SpawnPosition;
		//Spawn If is a empty location.
		SpawnPosition.RandomScale = FMath::RandRange(MinScale, MaxScale);
		
		bool bFound = FindEmptyLocation(SpawnPosition.RandomLocation, Radius*SpawnPosition.RandomScale);


		if (bFound)
		{
			SpawnPosition.RandomRotation = FMath::RandRange(-180.f, 180.f);

			PlaceActor(ToSpawn, SpawnPosition);// We can use the methon .Push() too for add an element to the TArray
			
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

//Using of Polymorphism
void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{

	FRotator Rotation = FRotator(0, SpawnPosition.RandomRotation, 0);
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn,SpawnPosition.RandomLocation, Rotation);
	if (!Spawned) { return; }

	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	

	//Asegura que APawn esta poseido
	Spawned->SpawnDefaultController();

	//Asegura que todos los APawn controllados por la AI son Enemy
	Spawned->Tags.Add(FName("Enemy"));
}

//Using of Polymorphism
void ATile::PlaceActor(TSubclassOf<AActor>ToSpawn, const FSpawnPosition SpawnPosition)
{

	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (!Spawned) { return; }
	Spawned->SetActorRelativeLocation(SpawnPosition.RandomLocation);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorScale3D(FVector(SpawnPosition.RandomScale));

	if (SpawnPosition.bOnlyRandRotationYaw)
	{
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.RandomRotation, 0));
	}

	else
	{
		Spawned->SetActorRotation(FRotator(SpawnPosition.RandomRotation));
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
