// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBTTask_RequestExitLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"
#include "UniqueNPC/Subsystem/NPCDirectorSubsystem.h"


EBTNodeResult::Type UNPCBTTask_RequestExitLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto Director = UNPCGameplayLibrary::GetNPCDirectorSubsystem(GetWorld()))
	{
		if (const auto RandomExit = Director->GetRandomExitLocation())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, RandomExit->GetActorLocation());
			return EBTNodeResult::Succeeded;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UNPCBTTask_RequestExitLocation Failed to find random exit location"));
	return EBTNodeResult::Failed;
	
}
