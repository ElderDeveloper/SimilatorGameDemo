// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NPCGameplayLibrary.generated.h"

struct FInventoryItem;
class UNPCPlayerInventorySubsystem;
class UNPCDirectorSubsystem;
/**
 * 
 */
UCLASS()
class UNIQUENPC_API UNPCGameplayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "NPC")
	static UNPCDirectorSubsystem* GetNPCDirectorSubsystem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "NPC")
	static UNPCPlayerInventorySubsystem* GetNPCPlayerInventorySubsystem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "NPC")
	static FInventoryItem GetItemDetails(FGameplayTag ItemTag);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "NPC")
	static void GiveRandomItemToPlayer(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "NPC")
	static FInventoryItem GetRandomItem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "NPC")
	static void SpawnRandomItem(const UObject* WorldContextObject, const FVector& Location, const FRotator& Rotation);
};

