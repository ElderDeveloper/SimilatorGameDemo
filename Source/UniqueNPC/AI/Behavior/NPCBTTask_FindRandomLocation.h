// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NPCBTTask_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class UNIQUENPC_API UNPCBTTask_FindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UNPCBTTask_FindRandomLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Blackboard")
	float Radius = 1000.0f;
};
