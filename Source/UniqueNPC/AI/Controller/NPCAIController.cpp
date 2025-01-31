// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCAIController.h"
#include "UniqueNPC/AI/NPCCharacter.h"

// Sets default values
ANPCAIController::ANPCAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (const auto NPCCharacter = Cast<ANPCCharacter>(InPawn))
	{
		RunBehaviorTree( NPCCharacter->GetBehaviorTree());
	}
}


