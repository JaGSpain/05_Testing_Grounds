// Copyleft

#include "GrassComponent.h"


// Sets default values for this component's properties
UGrassComponent::UGrassComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrassComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrass();
	
}


// Called every frame
void UGrassComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGrassComponent::SpawnGrass()
{
	for (size_t i = 0; i < SpawnCount; i++)
	{
		
		float RandRotation = FMath::RandRange(-180, 180);
		FVector InLocation = FMath::RandPointInBox(SpawningExtents);
		FVector InScale3D = FVector(FMath::RandRange(MinScale,MaxScale));
		//Rotation Only in the Yaw 
		FRotator InRotation = FRotator(0,RandRotation , 0);
		AddInstance(FTransform(InRotation,InLocation,InScale3D));
		
		//Random Scale to the object
		float RandomScale = FMath::RandRange(MinScale, MaxScale);




	}

}