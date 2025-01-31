// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UNPCBTTask_FindRandomLocation::UNPCBTTask_FindRandomLocation()
{
}

EBTNodeResult::Type UNPCBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// Get the AI controller
	if (AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		if (AActor* Actor = Cast<AActor>(AIController->GetPawn()))
		{
			if (auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld()))
			{
				// Get the NPC character's location
				const FVector Origin = Actor->GetActorLocation();
	
				// Get a random location
				FNavLocation Location;
				if (NavSystem->GetRandomReachablePointInRadius(Origin, Radius,Location, nullptr, nullptr))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, Location);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UNPCBTTask_FindRandomLocation Failed to find random location"));
	return EBTNodeResult::Succeeded;
}
