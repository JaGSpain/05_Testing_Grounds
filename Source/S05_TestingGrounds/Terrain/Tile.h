// Copyleft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

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
	
	int MinSpawn = 1;
	int MaxSpawn = 1;
	float Radius = 500.f;
	float MinScale = 1.f;
	float MaxScale = 1.f;

	virtual void BeginPlay();
public:	
	// Called every frame
	

private:
	
	//I will use the same idea as GetWorld()->SweepSingleByChanne() letting me fail in the empty location vector or geting the vector 
	//and the true value.
	//If you check the parameters in SweepSingleByChannel definition we can see it use Vector& HitLocation
	//So we'll doo the same here
	bool FindEmptyLocation(FVector& OutLocation,float Radius);
	void PlaceActor(TSubclassOf<AActor>ToSpawn, FVector SpawnPoint, bool bOnlyRandRotationYaw, float RandomRotation, float RandomScale);
	bool CanSpawnAtLocation(FVector Location, float Radius);
	
	bool bOnlyRandRotationYaw;//Used For give randon rotation in Yaw axis only
	


};
