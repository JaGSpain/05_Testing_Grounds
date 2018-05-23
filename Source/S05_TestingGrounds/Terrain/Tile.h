// Copyleft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()//For this parameters we can use the method of FTransform instead
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()
	

	bool bOnlyRandRotationYaw;
	FVector RandomLocation;
	float RandomRotation;
	float RandomScale;
	
};


class UActorPool;


UCLASS()
class S05_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale, bool bOnlyRandRotationYaw);
		

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtents;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtents;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffSet;

	
	//UPROPERTY(EditDefaultsOnly, Category = "Spawn Parameters")
	int MinSpawn = 1;

	//UPROPERTY(EditDefaultsOnly, Category = "Spawn Parameters")
		int MaxSpawn = 1;

	//UPROPERTY(EditDefaultsOnly, Category = "Spawn Parameters")
		float Radius = 500.0f;

	//UPROPERTY(EditDefaultsOnly, Category = "Spawn Parameters")
		float MinScale = 1.f;

	//UPROPERTY(EditDefaultsOnly, Category = "Spawn Parameters")
		float MaxScale = 1.f;

	//UPROPERTY(EditDefaultsOnly, Category = "Spawn Parameters")
	


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

	void PlaceActor(TSubclassOf<AActor>ToSpawn,const FSpawnPosition& SpawnPosition );

	bool CanSpawnAtLocation(FVector Location, float Radius);
	
	bool bOnlyRandRotationYaw;//Used For give randon rotation in Yaw axis only
	
	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;

	void PositionNavMeshBoundsVolume();

	TArray <FSpawnPosition> RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale, bool bOnlyRandRotationYaw);

};
