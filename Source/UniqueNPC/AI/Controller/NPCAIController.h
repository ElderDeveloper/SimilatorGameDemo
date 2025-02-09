// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

UCLASS()
class UNIQUENPC_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANPCAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
