// Copyleft

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


AActor* UActorPool::CheckOut()
{
	
		
	//Elimino elemento de la cadena TArray
	
	if (!Pool.Num()) 
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Non Elements in the pool."), *GetName());
		return nullptr; 
	}
			
		return Pool.Pop();

	
}

void UActorPool::Return(AActor* ActorToReturn)
{

	Add(ActorToReturn);

}

void  UActorPool::Add(AActor* ActorToAdd)
{

	Pool.Push(ActorToAdd);

}