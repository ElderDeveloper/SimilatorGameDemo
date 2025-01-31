// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NPCBTTask_ShoppingBehavior.generated.h"

class ANPCWaitLine;

UCLASS()
class UNIQUENPC_API UNPCBTTask_ShoppingBehavior : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UNPCBTTask_ShoppingBehavior();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector LineKey;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Blackboard")
	float AcceptanceRadius = 10.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Blackboard")
	bool PrintDistance = false;

	bool bReachedLine = false;
	FVector TargetLocation;
	
	UPROPERTY()
	ANPCWaitLine* WaitLine;
	
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
