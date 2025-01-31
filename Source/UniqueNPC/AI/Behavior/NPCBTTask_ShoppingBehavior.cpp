// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBTTask_ShoppingBehavior.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UniqueNPC/AI/Actors/NPCWaitLine.h"


UNPCBTTask_ShoppingBehavior::UNPCBTTask_ShoppingBehavior()
{
	NodeName = TEXT("Shopping Behavior");
	bNotifyTick = true;
	
}

EBTNodeResult::Type UNPCBTTask_ShoppingBehavior::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller
	if (const auto AIController = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		if (const auto Actor = AIController->GetPawn())
		{
			if ( const auto Line = Cast<ANPCWaitLine>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(LineKey.SelectedKeyName)))
			{
				WaitLine = Line;
			}
		}
	}
	return EBTNodeResult::InProgress;
}

void UNPCBTTask_ShoppingBehavior::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationKey.SelectedKeyName);

	if (AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		if (const auto Pawn = AIController->GetPawn())
		{
			AIController->MoveToLocation(TargetLocation, 10,false);
			float Distance = FVector::Dist(AIController->GetPawn()->GetActorLocation(), TargetLocation);
		
			if (Distance < AcceptanceRadius)
			{
				float TargetYaw = WaitLine->GetActorRotation().Yaw + 180;
				Pawn->SetActorRotation(FRotator(0.0f, TargetYaw, 0.0f));
			}
		
			if (PrintDistance)
			{
				UKismetSystemLibrary::DrawDebugSphere(this, TargetLocation, 10.0f, 12, FLinearColor::Green, 0.0f, 0.0f);
				FString DistanceString = FString::Printf(TEXT("Distance: %f"), Distance);
				GEngine->AddOnScreenDebugMessage( -1, 0.f, FColor::Red, DistanceString); 
			}
		}
	}
}
