// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NPCData.generated.h"


class ANPCItemBase;

USTRUCT(BlueprintType, Blueprintable)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category= "Item")
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item")
	int32 Quantity;

	// Items Details are not needed to be add in the inventory, but it will be filled by the DataTable
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item|Details")
	FName ItemName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item|Details")
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item|Details")
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item|Details")
	TSoftClassPtr<ANPCItemBase> ItemActorClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item|Details")
	UStaticMesh* ItemMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item|Details")
	bool bIsStackable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item|Details")
	uint8 MaxStackSize;

	FInventoryItem()
	{
		ItemTag = FGameplayTag();
		Quantity = 1;
		ItemName = NAME_None;
		Description = FString();
		Icon = nullptr;
		ItemActorClass = nullptr;
		ItemMesh = nullptr;
		bIsStackable = true;
		MaxStackSize = 255;
	}

	FInventoryItem(FGameplayTag Tag, uint8 Amount)
	{
		ItemTag = Tag;
		Quantity = Amount;
		ItemName = NAME_None;
		Description = FString();
		Icon = nullptr;
		ItemActorClass = nullptr;
		ItemMesh = nullptr;
		bIsStackable = true;
		MaxStackSize = 255;
	};
};

