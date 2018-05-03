// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"




EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//**********************
	//Get the control points
	//**********************
	
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	//Protect against no Patrol Route Component
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed;}

	
	//Obtencion de los Patrol Point definidos en Patrolling Guard (En unreal editor)
	// Warn if there's not patrol points assigned.
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
		{
		UE_LOG(LogTemp, Warning, TEXT("Guard:'%s' Need assign PatrolPoints!!!"), *(ControlledPawn->GetName()))
		return EBTNodeResult::Failed;
		}
			
	//**********************
	//Set next waypoints
	//**********************			

	auto BlackboardComp=OwnerComp.GetBlackboardComponent();
	//Necesito el indice como int
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	//Recuerda q los PatrolPoint (Actores) definidos en APatrollingGuard,
	//lo establecemos como Object como las cajas y demas elementos del entorno dando mayor versatilidad	
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	
	//***********************
	//Cycle the index
	//**********************
	auto NextIndex = (Index + 1) % PatrolPoints.Num(); //Num devuelve tamaño de vector, equivale a Length en BP
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;

}

