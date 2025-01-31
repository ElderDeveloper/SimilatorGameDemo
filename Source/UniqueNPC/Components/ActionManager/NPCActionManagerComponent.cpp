// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCActionManagerComponent.h"
#include "UniqueNPC/AI/Actions/NPCActionBase.h"


// Sets default values for this component's properties
UNPCActionManagerComponent::UNPCActionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


bool UNPCActionManagerComponent::ExecuteAction(TSubclassOf<UNPCActionBase> ActionClass)
{
	if (!ActionClass) return false;

	// Destroy the previous action if any
	if (CurrentAction.IsValid())
	{
		CurrentAction->Destroyed();
		CurrentAction->ConditionalBeginDestroy();
		CurrentAction.Reset();
	}

	// Spawn the new action
	CurrentAction = NewObject<UNPCActionBase>(this, ActionClass);
	if (CurrentAction.Get())
	{
		CurrentAction->Initialize(this);
		CurrentAction->Execute(GetOwner());
		return true;
	}

	return false;
}


bool UNPCActionManagerComponent::ExecuteActionByTag(const FGameplayTag ActionTag)
{
	TArray<TSubclassOf<UNPCActionBase>> Actions;
	for (const FNPCAction& Action : NPCActions)
	{
		if (Action.ActionTag.MatchesTagExact(ActionTag))
		{
			Actions.Add(Action.ActionClass);
		}
	}

	if (Actions.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, Actions.Num() - 1);
		return ExecuteAction(Actions[RandomIndex]);;
	}

	return false;
}


void UNPCActionManagerComponent::FinishAction(UNPCActionBase* FinishedAction)
{
	OnActionFinished.Broadcast(FinishedAction);
}