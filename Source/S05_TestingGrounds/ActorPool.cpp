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
	
	UE_LOG(LogTemp,Warning,TEXT("[%s] CHECKOUT."),*GetName())
	return nullptr;
}

void UActorPool::Return(AActor* ActorToReturn)
{

	if (!ActorToReturn) 
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Returned NullActor:"), *GetName());
	
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Actor returned:{%s}!!"), *GetName(),*ActorToReturn->GetName());
}

void  UActorPool::Add(AActor* ActorToAdd)
{

	if (!ActorToAdd)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Added Null Actor:"), *GetName());

		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Actor added:{%s}!!"), *GetName(), *ActorToAdd->GetName());


}