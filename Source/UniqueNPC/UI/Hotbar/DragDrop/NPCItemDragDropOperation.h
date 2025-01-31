// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "UniqueNPC/Data/NPCData.h"
#include "NPCItemDragDropOperation.generated.h"

class UNPCHotbarItemHolder;
/**
 * 
 */
UCLASS()
class UNIQUENPC_API UNPCItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FInventoryItem ItemData;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	UNPCHotbarItemHolder* SourceSlot;
	
};
