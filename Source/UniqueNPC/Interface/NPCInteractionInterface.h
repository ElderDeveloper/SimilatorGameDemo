// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPCInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UNPCInteractionInterface : public UInterface
{
	GENERATED_BODY()
};


class UNIQUENPC_API INPCInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC Interaction")
	bool OnInteract(class ACharacter* InteractingCharacter);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC Interaction")
	bool OnStopInteract(class ACharacter* InteractingCharacter);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC Interaction")
	bool CanInteract(class ACharacter* InteractingCharacter);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC Interaction")
	bool CanStopInteract(class ACharacter* InteractingCharacter);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC Interaction")
	bool IsNPCCharacter();

	
};
