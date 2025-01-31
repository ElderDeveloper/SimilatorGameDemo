// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPCWaitLineInterface.generated.h"

class ANPCWaitLine;
// This class does not need to be modified.
UINTERFACE()
class UNPCWaitLineInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNIQUENPC_API INPCWaitLineInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WaitLine")
	void MoveToLine(ANPCWaitLine* Line, const FVector TargetLocation, const uint8 LineIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WaitLine")
	void RequestWaitLine(ANPCWaitLine* Line);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WaitLine")
	bool IsShopping();
};
