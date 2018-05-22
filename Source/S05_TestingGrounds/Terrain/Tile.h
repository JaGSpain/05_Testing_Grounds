// Copyleft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


class UActorPool;


UCLASS()
class S05_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale,bool bOnlyRandRotationYaw);
		

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtents;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtents;


	int MinSpawn = 1;
	int MaxSpawn = 1;
	float Radius = 500.f;
	float MinScale = 1.f;
	float MaxScale = 1.f;

	virtual void BeginPlay()override;

	// Look in Actor life cycle
	//https://docs.unrealengine.com/en-us/Programming/UnrealArchitecture/Actors/ActorLifecycle

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* Pool);

			

private:
	
	//I will use the same idea as GetWorld()->SweepSingleByChanne() letting me fail in the empty location vector or geting the vector 
	//and the true value.
	//If you check the parameters in SweepSingleByChannel definition we can see it use Vector& HitLocation
	//So we'll doo the same here
	bool FindEmptyLocation(FVector& OutLocation,float Radius);
	void PlaceActor(TSubclassOf<AActor>ToSpawn, FVector SpawnPoint, bool bOnlyRandRotationYaw, float RandomRotation, float RandomScale);
	bool CanSpawnAtLocation(FVector Location, float Radius);
	
	bool bOnlyRandRotationYaw;//Used For give randon rotation in Yaw axis only
	
	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;

	void PositionNavMeshBoundsVolume();

};
