// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "NPCItemSettings.generated.h"

struct FInventoryItem;

UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Item Settings"))
class UNPCItemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config,BlueprintReadWrite, EditAnywhere, Category = "Item Settings")
	uint8 PlayerInventorySize = 10;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "NPC Settings")
	TSoftObjectPtr<UDataTable> NPCItemTable;
	
	UFUNCTION(BlueprintPure, Category = "Item Settings")
	FInventoryItem GetItemDetails(FGameplayTag ItemTag) const;
	
};
