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
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, bool bOnlyRandRotationYaw);
		

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	//I will use the same idea as GetWorld()->SweepSingleByChanne() letting me fail in the empty location vector or geting the vector 
	//and the true value.
	//If you check the parameters in SweepSingleByChannel definition we can see it use Vector& HitLocation
	//So we'll doo the same here
	bool FindEmptyLocation(FVector& OutLocation,float Radius);
	void PlaceActor(TSubclassOf<AActor>ToSpawn, FVector SpawnPoint, bool bOnlyRandRotationYaw);
	bool CanSpawnAtLocation(FVector Location, float Radius);
	
	bool bOnlyRandRotationYaw;//Used For give randon rotation in Yaw axis only
	float Radius = 300.f;
};
