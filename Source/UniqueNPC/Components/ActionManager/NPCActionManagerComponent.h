// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "NPCActionManagerComponent.generated.h"

class UNPCActionBase;

USTRUCT(BlueprintType,Blueprintable)
struct FNPCAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActionTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UNPCActionBase> ActionClass;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionExecuted, UNPCActionBase*, ExecutedAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionFinished, UNPCActionBase*, FinishedAction);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNIQUENPC_API UNPCActionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNPCActionManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FNPCAction> NPCActions;

	// Triggers an action by class type
	UFUNCTION(BlueprintCallable)
	bool ExecuteAction(TSubclassOf<UNPCActionBase> ActionClass);

	UFUNCTION(BlueprintCallable)
	bool ExecuteActionByTag(const FGameplayTag ActionTag);

	UFUNCTION(BlueprintCallable)
	void FinishAction(UNPCActionBase* FinishedAction);

	UPROPERTY(BlueprintAssignable)
	FOnActionExecuted OnActionExecuted;

	UPROPERTY(BlueprintAssignable)
	FOnActionFinished OnActionFinished;

	// Returns the currently executing action , We can not return TWeakObjectPtr directly to blueprint
	UFUNCTION(BlueprintPure)
	UNPCActionBase* GetCurrentAction() const { return CurrentAction.Get(); }

private:
	UPROPERTY()
	TWeakObjectPtr<UNPCActionBase> CurrentAction;
};


