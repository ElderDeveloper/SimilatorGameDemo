// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NPCBTTask_RequestExitLocation.generated.h"

/**
 * 
 */
UCLASS()
class UNIQUENPC_API UNPCBTTask_RequestExitLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;
};
